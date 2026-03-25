#pragma once
/**
 * PC stub for dolphin/G2D.h
 * All types are kept identical to the original.
 * The rendering functions are no-ops — G2D tile/sprite rendering was a
 * thin wrapper around GX calls; on PC the game uses its own render path.
 */

#include "dolphin/gx.h"

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------
typedef enum G2DMatCtg {
    G2D_CTG_TEXTURE,
    G2D_CTG_RGB_DIRECT,
    G2D_CTG_RGBA_INDEX8,
    G2D_CTG_EMPTY,
} G2DMatCtg;

typedef struct G2DSprite {
    u16      nTlcS;     // Top-left corner S
    u16      nTlcT;     // Top-left corner T
    u16      nWidth;
    u16      nHeight;
    GXTexObj* to;
    f32      rS0;
    f32      rT0;
    f32      rS1;
    f32      rT1;
} G2DSprite;

typedef struct G2DPosOri {
    f32 rPosX;
    f32 rPosY;
    f32 rOriX;
    f32 rOriY;
} G2DPosOri;

typedef struct G2DGlob {
    u16      nViewportTlcX;
    u16      nViewportTlcY;
    u16      nViewportWidth;
    u16      nViewportHeight;
    G2DPosOri poCam;
    f32      rWorldX;
    f32      rWorldY;
    f32      rHalfX;
    f32      rHalfY;
} G2DGlob;

typedef struct G2DMatDesc {
    s32       nReserved;
    G2DMatCtg nCategory;
    GXColor*  color;
    GXTexObj* to;
    u8*       clut;  // Color Look-Up Table
} G2DMatDesc;

typedef struct G2DTileDesc {
    u8 nMaterial;
    u8 nS;
    u8 nT;
    u8 nCI;       // Color Index
    u8 aUser[4];
} G2DTileDesc;

typedef struct G2DLayer {
    void*       map;
    s8          nHS;
    s8          nVS;
    s8          nBPI;
    s16         nTileWidth;
    s16         nTileHeight;
    s8          bWrap;
    u8          nNumMaterials;
    G2DTileDesc* tileDesc;
    G2DMatDesc*  matDesc;
} G2DLayer;

// ---------------------------------------------------------------------------
// Function stubs
// ---------------------------------------------------------------------------
static inline void G2DInitSprite(G2DSprite* sprite)                       { (void)sprite; }
static inline void G2DDrawSprite(G2DSprite* sprite, G2DPosOri* po)        { (void)sprite; (void)po; }
static inline void G2DDrawLayer(G2DLayer* layer, s8* aSortBuffer)         { (void)layer; (void)aSortBuffer; }
static inline void G2DSetCamera(G2DPosOri* po)                            { (void)po; }
static inline void G2DInitWorld(u32 nWorldX, u32 nWorldY)                 { (void)nWorldX; (void)nWorldY; }
static inline void G2DSetViewport(u16 nLeft, u16 nTop, u16 nWidth, u16 nHeight)
    { (void)nLeft; (void)nTop; (void)nWidth; (void)nHeight; }

#ifdef __cplusplus
}
#endif
