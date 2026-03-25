/**
 * src/dolphin/texture.cpp
 * GameCube texture format untiling and conversion.
 *
 * GC textures are stored in 4×4 tile blocks (each tile = one cache line).
 * Before uploading to OpenGL we must untile them to standard scanline order.
 *
 * Supported formats and their GL equivalents:
 *   I4    → GL_R8      (4-bit intensity, expand to 8-bit)
 *   I8    → GL_R8
 *   IA4   → GL_RG8     (4-bit intensity + alpha)
 *   IA8   → GL_RG8
 *   RGB565→ GL_RGB5    (16-bit colour)
 *   RGB5A3→ GL_RGBA8   (15-bit colour + optional 3-bit alpha)
 *   RGBA8 → GL_RGBA8   (32-bit colour, split into two sub-tiles)
 *   CMPR  → expand to RGBA8 (GC S3TC / DXT1 variant — different block layout)
 *   CI4/CI8 → expand via TLUT to RGBA8
 */

#include "port/port.h"
#include "dolphin/gx/GXEnum.h"

#include <cstdint>
#include <cstring>
#include <vector>

// -----------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------
static inline uint8_t  extend4to8(uint8_t v)  { return (v << 4) | v; }
static inline uint8_t  extend3to8(uint8_t v)  { return (v << 5) | (v << 2) | (v >> 1); }
static inline uint8_t  extend5to8(uint8_t v)  { return (v << 3) | (v >> 2); }
static inline uint8_t  extend6to8(uint8_t v)  { return (v << 2) | (v >> 4); }

// -----------------------------------------------------------------------
// Tile coordinate → linear index
// For a WxH texture the GC layout groups pixels into 4×4 tiles,
// tiles are laid out in scanline order within the texture.
// -----------------------------------------------------------------------
static inline int TiledOffset(int x, int y, int w, int tileW, int tileH) {
    int tileX   = x / tileW;
    int tileY   = y / tileH;
    int tilesX  = (w + tileW - 1) / tileW;
    int inTileX = x % tileW;
    int inTileY = y % tileH;
    return (tileY * tilesX + tileX) * (tileW * tileH) + inTileY * tileW + inTileX;
}

// -----------------------------------------------------------------------
// Per-format untilers
// -----------------------------------------------------------------------

// I4 → R8 (1 byte per 2 pixels in source)
static void UntileI4(const uint8_t* src, uint8_t* dst, int w, int h) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int off  = TiledOffset(x, y, w, 8, 8);
            uint8_t nibble = (x & 1) ? (src[off/2] & 0x0F) : (src[off/2] >> 4);
            dst[y*w + x] = extend4to8(nibble);
        }
    }
}

// I8 → R8
static void UntileI8(const uint8_t* src, uint8_t* dst, int w, int h) {
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            dst[y*w + x] = src[TiledOffset(x, y, w, 8, 4)];
}

// IA4 → RG8 (4-bit I + 4-bit A per pixel)
static void UntileIA4(const uint8_t* src, uint8_t* dst, int w, int h) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            uint8_t b = src[TiledOffset(x, y, w, 8, 4)];
            dst[(y*w + x)*2 + 0] = extend4to8(b >> 4);   // intensity
            dst[(y*w + x)*2 + 1] = extend4to8(b & 0x0F); // alpha
        }
    }
}

// IA8 → RG8 (8-bit I + 8-bit A per pixel)
static void UntileIA8(const uint8_t* src, uint8_t* dst, int w, int h) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int off = TiledOffset(x, y, w, 4, 4) * 2;
            dst[(y*w + x)*2 + 0] = src[off + 0]; // intensity
            dst[(y*w + x)*2 + 1] = src[off + 1]; // alpha
        }
    }
}

// RGB565 → RGBA8 (expand for GL compatibility)
static void UntileRGB565(const uint8_t* src, uint8_t* dst, int w, int h) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int off = TiledOffset(x, y, w, 4, 4) * 2;
            uint16_t px = ((uint16_t)src[off] << 8) | src[off+1];
            int base = (y*w + x)*4;
            dst[base+0] = extend5to8((px >> 11) & 0x1F); // R
            dst[base+1] = extend6to8((px >>  5) & 0x3F); // G
            dst[base+2] = extend5to8( px        & 0x1F); // B
            dst[base+3] = 255;
        }
    }
}

// RGB5A3 → RGBA8
// If bit 15 = 1: RGB555 (A=255)
// If bit 15 = 0: RGB444 + A3
static void UntileRGB5A3(const uint8_t* src, uint8_t* dst, int w, int h) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int off = TiledOffset(x, y, w, 4, 4) * 2;
            uint16_t px = ((uint16_t)src[off] << 8) | src[off+1];
            int base = (y*w + x)*4;
            if (px & 0x8000) {
                dst[base+0] = extend5to8((px >> 10) & 0x1F);
                dst[base+1] = extend5to8((px >>  5) & 0x1F);
                dst[base+2] = extend5to8( px        & 0x1F);
                dst[base+3] = 255;
            } else {
                dst[base+3] = extend3to8((px >> 12) & 0x07);
                dst[base+0] = extend4to8((px >>  8) & 0x0F);
                dst[base+1] = extend4to8((px >>  4) & 0x0F);
                dst[base+2] = extend4to8( px        & 0x0F);
            }
        }
    }
}

// RGBA8 — two 4×4 sub-tiles: first tile stores AR bytes, second stores GB bytes
static void UntileRGBA8(const uint8_t* src, uint8_t* dst, int w, int h) {
    int tilesX = (w + 3) / 4;
    int tilesY = (h + 3) / 4;
    for (int tileY = 0; tileY < tilesY; tileY++) {
        for (int tileX = 0; tileX < tilesX; tileX++) {
            int tileBase = (tileY * tilesX + tileX) * 64; // 4×4 pixels × 2 sub-tiles × 2 bytes = 64
            for (int inY = 0; inY < 4; inY++) {
                for (int inX = 0; inX < 4; inX++) {
                    int px = tileY*4 + inY;
                    int py = tileX*4 + inX;
                    if (px >= h || py >= w) continue;
                    int srcIdx = tileBase + inY*4 + inX;
                    int dstIdx = (px*w + py) * 4;
                    dst[dstIdx+3] = src[srcIdx*2 + 0]; // A
                    dst[dstIdx+0] = src[srcIdx*2 + 1]; // R
                    dst[dstIdx+1] = src[srcIdx*2 + 32]; // G (second sub-tile)
                    dst[dstIdx+2] = src[srcIdx*2 + 33]; // B
                }
            }
        }
    }
}

// CMPR (GC S3TC / DXT1 variant)
// GC groups 4 DXT1 blocks into a 2×2 meta-block; pixel order within each
// block also differs from PC DXT1.
static uint32_t CMPRInterpolate(uint16_t c0, uint16_t c1, int idx) {
    uint8_t r0 = extend5to8((c0>>11)&0x1F), g0 = extend6to8((c0>>5)&0x3F), b0 = extend5to8(c0&0x1F);
    uint8_t r1 = extend5to8((c1>>11)&0x1F), g1 = extend6to8((c1>>5)&0x3F), b1 = extend5to8(c1&0x1F);
    uint8_t r,g,b,a=255;
    if (c0 > c1) {
        switch(idx) {
            case 0: r=r0;g=g0;b=b0; break;
            case 1: r=r1;g=g1;b=b1; break;
            case 2: r=(2*r0+r1)/3;g=(2*g0+g1)/3;b=(2*b0+b1)/3; break;
            default:r=(r0+2*r1)/3;g=(g0+2*g1)/3;b=(b0+2*b1)/3; break;
        }
    } else {
        switch(idx) {
            case 0: r=r0;g=g0;b=b0; break;
            case 1: r=r1;g=g1;b=b1; break;
            case 2: r=(r0+r1)/2;g=(g0+g1)/2;b=(b0+b1)/2; break;
            default:r=g=b=0;a=0; break;
        }
    }
    return (uint32_t)r|(uint32_t)g<<8|(uint32_t)b<<16|(uint32_t)a<<24;
}

static void UntileCMPR(const uint8_t* src, uint8_t* dst, int w, int h) {
    // GC CMPR: 2×2 grid of DXT1 sub-blocks per 8×8 meta-tile
    int metaTilesX = (w + 7) / 8;
    int metaTilesY = (h + 7) / 8;
    for (int mtY = 0; mtY < metaTilesY; mtY++) {
        for (int mtX = 0; mtX < metaTilesX; mtX++) {
            for (int sbY = 0; sbY < 2; sbY++) {
                for (int sbX = 0; sbX < 2; sbX++) {
                    int blockOff = ((mtY*metaTilesX + mtX)*4 + sbY*2 + sbX) * 8;
                    uint16_t c0 = ((uint16_t)src[blockOff+0]<<8)|src[blockOff+1];
                    uint16_t c1 = ((uint16_t)src[blockOff+2]<<8)|src[blockOff+3];
                    for (int row = 0; row < 4; row++) {
                        uint8_t bits = src[blockOff+4+row];
                        for (int col = 0; col < 4; col++) {
                            int px = mtX*8 + sbX*4 + col;
                            int py = mtY*8 + sbY*4 + row;
                            if (px >= w || py >= h) continue;
                            int idx = (bits >> (6 - col*2)) & 3;
                            uint32_t rgba = CMPRInterpolate(c0, c1, idx);
                            int dstOff = (py*w + px)*4;
                            dst[dstOff+0] =  rgba        & 0xFF;
                            dst[dstOff+1] = (rgba >>  8) & 0xFF;
                            dst[dstOff+2] = (rgba >> 16) & 0xFF;
                            dst[dstOff+3] = (rgba >> 24) & 0xFF;
                        }
                    }
                }
            }
        }
    }
}

// -----------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------

namespace tp::texture {

/**
 * Untile a GameCube texture into a flat RGBA8 buffer suitable for glTexImage2D.
 *
 * @param srcFmt   GX texture format (GXTexFmt enum value)
 * @param src      pointer to raw tiled texture data
 * @param w        texture width in pixels
 * @param h        texture height in pixels
 * @param dst      output buffer (must be at least w*h*4 bytes)
 * @param dstFmt   receives the GL internal format to use
 * @param glFmt    receives the GL external format (GL_RGBA, GL_RG, GL_RED, ...)
 * @param dataType receives the GL data type (GL_UNSIGNED_BYTE, ...)
 */
void Untile(int srcFmt, const void* src, int w, int h, void* dst,
            int* dstInternalFmt, int* glFmt, int* glDataType)
{
    const uint8_t* s = (const uint8_t*)src;
    uint8_t*       d = (uint8_t*)dst;

    // Default: RGBA8
    *dstInternalFmt = 0x8058; // GL_RGBA8
    *glFmt          = 0x1908; // GL_RGBA
    *glDataType     = 0x1401; // GL_UNSIGNED_BYTE

    switch (srcFmt) {
        case 0x0: // GX_TF_I4
            UntileI4(s, d, w, h);
            *dstInternalFmt = 0x8229; // GL_R8
            *glFmt          = 0x1903; // GL_RED
            break;
        case 0x1: // GX_TF_I8
            UntileI8(s, d, w, h);
            *dstInternalFmt = 0x8229;
            *glFmt          = 0x1903;
            break;
        case 0x2: // GX_TF_IA4
            UntileIA4(s, d, w, h);
            *dstInternalFmt = 0x822B; // GL_RG8
            *glFmt          = 0x8227; // GL_RG
            break;
        case 0x3: // GX_TF_IA8
            UntileIA8(s, d, w, h);
            *dstInternalFmt = 0x822B;
            *glFmt          = 0x8227;
            break;
        case 0x4: // GX_TF_RGB565
            UntileRGB565(s, d, w, h);
            break;
        case 0x5: // GX_TF_RGB5A3
            UntileRGB5A3(s, d, w, h);
            break;
        case 0x6: // GX_TF_RGBA8
            UntileRGBA8(s, d, w, h);
            break;
        case 0xE: // GX_TF_CMPR
            UntileCMPR(s, d, w, h);
            break;
        default:
            // Unknown format — fill magenta as placeholder
            for (int i = 0; i < w*h; i++) { d[i*4]=255; d[i*4+1]=0; d[i*4+2]=255; d[i*4+3]=255; }
            break;
    }
}

/** Returns bytes-per-texel in the GC tiled format (useful for mip sizing) */
int BytesPerPixel(int srcFmt) {
    switch (srcFmt) {
        case 0x0: return 0; // I4  — 0.5 bpp, handle specially
        case 0x1: return 1; // I8
        case 0x2: return 1; // IA4
        case 0x3: return 2; // IA8
        case 0x4: return 2; // RGB565
        case 0x5: return 2; // RGB5A3
        case 0x6: return 4; // RGBA8
        case 0xE: return 0; // CMPR — 0.5 bpp, handled per-block
        default:  return 4;
    }
}

} // namespace tp::texture
