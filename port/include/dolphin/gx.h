#pragma once
/**
 * PC stub for dolphin/gx.h
 * Provides the GX API surface backed by OpenGL 4.5.
 *
 * GX is a tile-based, immediate-mode GPU API designed around the
 * GameCube's embedded GPU (Flipper/Hollywood).  This header re-exposes
 * the same function signatures so the game source compiles unchanged;
 * src/dolphin/gx.cpp translates each call to OpenGL equivalents.
 *
 * Key mappings:
 *   GXInit          → create GL context (done by window layer)
 *   GXSetViewport   → glViewport
 *   GXSetScissor    → glScissor
 *   GXBegin/GXEnd   → buffer vertex data, draw with glDrawArrays
 *   GXLoadTexObj    → glBindTexture
 *   GXSetTevStages  → GLSL shader permutations
 *   GXCopyDisp      → blit framebuffer to screen
 *   GXClearColor    → glClearColor
 */

// Pull in GX enum types from the decomp (they're just integers, no hardware deps)
#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>
#include "dolphin/os/OSThread.h"

#ifndef CP_REG_MTXIDXA_ID
#define CP_REG_MTXIDXA_ID 0x30
#endif

#ifndef CP_REG_MTXIDXB_ID
#define CP_REG_MTXIDXB_ID 0x40
#endif

#ifndef GX_XF_REG_MATRIXINDEX0
#define GX_XF_REG_MATRIXINDEX0 0x1018
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PPCWGPipe {
    u8 u8;
    u16 u16;
    u32 u32;
    f32 f32;
} PPCWGPipe;

extern PPCWGPipe GXWGFifo;

static inline void GXCmd1u8(u8 value)   { GXWGFifo.u8 = value; }
static inline void GXCmd1u16(u16 value) { GXWGFifo.u16 = value; }
static inline void GXCmd1u32(u32 value) { GXWGFifo.u32 = value; }

// -----------------------------------------------------------------------
// Init / management
// -----------------------------------------------------------------------
typedef void (*GXDrawSyncCallback)(u16 token);
typedef void (*GXDrawDoneCallback)(void);

void  GXInit(void* base, u32 size);  // base/size ignored on PC
void  GXFlush(void);
void  GXDrawDone(void);
void  GXWaitDrawDone(void);
void  GXSetDrawSync(u16 token);
u16   GXReadDrawSync(void);
void  GXSetDrawDone(void);
void  GXAbortFrame(void);
void  GXPixModeSync(void);
void  GXTexModeSync(void);
GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback cb);
GXDrawDoneCallback GXSetDrawDoneCallback(GXDrawDoneCallback cb);
static inline OSThread* GXGetCurrentGXThread(void) { return nullptr; }
static inline void GXSetCurrentGXThread(void) {}

// -----------------------------------------------------------------------
// Viewport / scissor
// -----------------------------------------------------------------------
void GXSetViewport(f32 xOrig, f32 yOrig, f32 wd, f32 ht, f32 nearZ, f32 farZ);
void GXSetViewportJitter(f32 xOrig, f32 yOrig, f32 wd, f32 ht, f32 nearZ, f32 farZ, u32 field);
void GXGetViewportv(f32* ptr);
void GXSetScissor(u32 xOrig, u32 yOrig, u32 wd, u32 ht);
void GXSetScissorBoxOffset(s32 xOffset, s32 yOffset);
void GXEnableTexOffsets(GXTexCoordID coord, GXBool line_enable, GXBool point_enable);
void GXInvalidateVtxCache(void);

// -----------------------------------------------------------------------
// Framebuffer / copy
// -----------------------------------------------------------------------
void GXCopyDisp(void* dest, GXBool clear);
void GXCopyTex(void* dest, GXBool clear);
void GXSetCopyFilter(GXBool aa, const u8 sample_pattern[12][2], GXBool vf, const u8 vfilter[7]);
void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht);
void GXSetTexCopyDst(u16 wd, u16 ht, GXTexFmt fmt, GXBool mipmap);
void GXClearBoundingBox(void);
void GXReadBoundingBox(u16* top, u16* bottom, u16* left, u16* right);
void GXSetColorUpdate(GXBool enable);
void GXSetAlphaUpdate(GXBool enable);

// Clear values
void GXSetCopyClear(GXColor color, u32 zValue);

// -----------------------------------------------------------------------
// Z-buffer
// -----------------------------------------------------------------------
void GXSetZMode(GXBool enable, GXCompare func, GXBool updateEnable);
void GXSetZCompLoc(GXBool before_tex);
void GXSetZTexture(GXZTexOp op, GXTexFmt fmt, u32 bias);

// -----------------------------------------------------------------------
// Blend / alpha
// -----------------------------------------------------------------------
void GXSetBlendMode(GXBlendMode type, GXBlendFactor src, GXBlendFactor dst, GXLogicOp op);
void GXSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1, u8 ref1);
void GXSetDstAlpha(GXBool enable, u8 a);

// -----------------------------------------------------------------------
// Culling / rasterizer
// -----------------------------------------------------------------------
void GXSetCullMode(GXCullMode mode);
void GXSetLineWidth(u8 width, GXTexOffset texOffsets);
void GXSetPointSize(u8 pointSize, GXTexOffset texOffsets);
void GXSetFieldMask(GXBool odd, GXBool even);
void GXSetFieldMode(GXBool field, GXBool halfAspectRatio);

// -----------------------------------------------------------------------
// Vertex descriptor / attribute format
// -----------------------------------------------------------------------
void GXClearVtxDesc(void);
void GXSetVtxDesc(GXAttr attr, GXAttrType type);
void GXSetVtxDescv(const GXVtxDescList* attrList);
void GXSetVtxAttrFmt(GXVtxFmt vtxfmt, GXAttr attr, GXCompCnt cnt, GXCompType type, u8 frac);
void GXSetVtxAttrFmtv(GXVtxFmt vtxfmt, const GXVtxAttrFmtList* attrList);
void GXSetArray(GXAttr attr, void* ptr, u8 stride);

// -----------------------------------------------------------------------
// Immediate-mode geometry (translates to buffered GL draw calls)
// -----------------------------------------------------------------------
void GXBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 vtxCnt);
void GXEnd(void);  // submits buffered geometry

// Vertex attribute submission (called between GXBegin/GXEnd)
void GXPosition3f32(f32 x, f32 y, f32 z);
void GXPosition3s16(s16 x, s16 y, s16 z);
void GXPosition3s8(s8 x, s8 y, s8 z);
void GXPosition2f32(f32 x, f32 y);
void GXPosition2s16(s16 x, s16 y);
void GXPosition2s8(s8 x, s8 y);
void GXPosition1x8(u8 idx);
void GXPosition1x16(u16 idx);
void GXNormal3f32(f32 nx, f32 ny, f32 nz);
void GXNormal3s16(s16 nx, s16 ny, s16 nz);
void GXNormal3s8(s8 nx, s8 ny, s8 nz);
void GXNormal1x8(u8 idx);
void GXNormal1x16(u16 idx);
void GXColor4u8(u8 r, u8 g, u8 b, u8 a);
void GXColor3u8(u8 r, u8 g, u8 b);
void GXColor3f32(f32 r, f32 g, f32 b);
void GXColor4f32(f32 r, f32 g, f32 b, f32 a);
void GXColor1u32(u32 clr);
void GXColor1u16(u16 clr);
void GXColor1x8(u8 idx);
void GXColor1x16(u16 idx);
void GXTexCoord2f32(f32 s, f32 t);
void GXTexCoord2s16(s16 s, s16 t);
void GXTexCoord2s8(s8 s, s8 t);
void GXTexCoord1x8(u8 idx);
void GXTexCoord1x16(u16 idx);

// Display lists
void* GXBeginDisplayList(void* list, u32 size);
u32   GXEndDisplayList(void);
void  GXCallDisplayList(const void* list, u32 nbytes);

// -----------------------------------------------------------------------
// Texture
// -----------------------------------------------------------------------
void GXInitTexObj(GXTexObj* obj, void* imagePtr, u16 wd, u16 ht,
                  GXTexFmt fmt, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXBool mipMap);
void GXInitTexObjCI(GXTexObj* obj, void* imagePtr, u16 wd, u16 ht,
                    GXCITexFmt fmt, GXTexWrapMode wrapS, GXTexWrapMode wrapT,
                    GXBool mipMap, u32 tlutName);
void GXInitTexObjLOD(GXTexObj* obj, GXTexFilter minFilt, GXTexFilter magFilt,
                     f32 minLod, f32 maxLod, f32 lodBias,
                     GXBool biasClamp, GXBool doEdgeLOD, GXAnisotropy maxAniso);
void GXLoadTexObj(GXTexObj* obj, GXTexMapID id);
void GXDestroyTexObj(GXTexObj* obj);
void GXInvalidateTexAll(void);
void GXInitTlutObj(GXTlutObj* tlutObj, void* lut, GXTlutFmt fmt, u16 entries);
void GXLoadTlut(GXTlutObj* tlutObj, u32 tlutName);
u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, GXBool mipMap, u8 maxLod);
static inline u16 GXGetTexObjWidth(const GXTexObj* /*obj*/) { return 0; }
static inline u16 GXGetTexObjHeight(const GXTexObj* /*obj*/) { return 0; }
static inline void GXSetTexCoordScaleManually(GXTexCoordID /*coord*/, u8 /*enable*/, u16 /*ss*/, u16 /*ts*/) {}

// -----------------------------------------------------------------------
// TEV (Texture Environment) — translated to GLSL shader permutations
// -----------------------------------------------------------------------
void GXSetNumTevStages(u8 num);
void GXSetTevOrder(GXTevStageID stage, GXTexCoordID texcoord,
                   GXTexMapID texmap, GXChannelID color);
void GXSetTevOp(GXTevStageID stage, GXTevMode mode);
void GXSetTevColorIn(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b,
                     GXTevColorArg c, GXTevColorArg d);
void GXSetTevAlphaIn(GXTevStageID stage, GXTevAlphaArg a, GXTevAlphaArg b,
                     GXTevAlphaArg c, GXTevAlphaArg d);
void GXSetTevColorOp(GXTevStageID stage, GXTevOp op, GXTevBias bias,
                     GXTevScale scale, GXBool clamp, GXTevRegID outReg);
void GXSetTevAlphaOp(GXTevStageID stage, GXTevOp op, GXTevBias bias,
                     GXTevScale scale, GXBool clamp, GXTevRegID outReg);
void GXSetTevColor(GXTevRegID id, GXColor color);
void GXSetTevColorS10(GXTevRegID id, GXColorS10 color);
void GXSetTevDirect(GXTevStageID tev_stage);
void GXSetTevKColor(GXTevKColorID id, GXColor color);
void GXSetTevKColorSel(GXTevStageID stage, GXTevKColorSel sel);
void GXSetTevKAlphaSel(GXTevStageID stage, GXTevKAlphaSel sel);
void GXSetTevSwapMode(GXTevStageID stage, GXTevSwapSel rasSel, GXTevSwapSel texSel);
void GXSetTevSwapModeTable(GXTevSwapSel id, GXTevColorChan red, GXTevColorChan green,
                            GXTevColorChan blue, GXTevColorChan alpha);
void GXSetTevIndirect(GXTevStageID tevstage, GXIndTexStageID indstage,
                      GXIndTexFormat fmt, GXIndTexBiasSel biasSel,
                      GXIndTexMtxID mtxSel, GXIndTexWrap wrapS,
                      GXIndTexWrap wrapT, GXBool addPrev, GXBool utcLod,
                      GXIndTexAlphaSel alphaSel);
void GXSetNumIndStages(u8 nstages);
void GXSetIndTexCoordScale(GXIndTexStageID indstage, GXIndTexScale scaleS, GXIndTexScale scaleT);
void GXSetIndTexOrder(GXIndTexStageID indstage, GXTexCoordID texcoord, GXTexMapID texmap);
void GXSetIndTexMtx(GXIndTexMtxID mtxSelID, const f32 offset[2][3], s8 scaleExp);
void GXSetIndTexMatrix(GXIndTexMtxID mtxSelID, const f32 offset[2][3], s8 scaleExp);

// -----------------------------------------------------------------------
// Transforms & matrices
// -----------------------------------------------------------------------
void GXLoadPosMtxImm(const f32 mt[3][4], u32 pnidx);
void GXLoadNrmMtxImm(const f32 mt[3][4], u32 pnidx);
void GXLoadTexMtxImm(const f32 mt[][4], u32 id, GXTexMtxType type);
void GXLoadPosMtxIndx(u16 mtxidx, u32 pnidx);
void GXLoadNrmMtxIndx3x3(u16 mtxidx, u32 pnidx);
void GXLoadTexMtxIndx(u16 mtxidx, u32 id, GXTexMtxType type);
void GXSetCurrentMtx(u32 mtx);

void GXSetProjection(const f32 mtx[4][4], GXProjectionType type);
void GXSetProjectionv(const f32* ptr, GXProjectionType type);
void GXGetProjectionv(f32* ptr);
void GXProject(f32 x, f32 y, f32 z, const Mtx modelMtx, const f32* proj, const f32* viewport,
               f32* winx, f32* winy, f32* winz);
void GXSetClipMode(GXClipMode mode);
void GXSetCoPlanar(GXBool enable);

// -----------------------------------------------------------------------
// Lighting
// -----------------------------------------------------------------------
void GXSetNumChans(u8 nChans);
void GXSetChanCtrl(GXChannelID chan, GXBool enable, GXColorSrc ambSrc, GXColorSrc matSrc,
                   u32 lightMask, GXDiffuseFn diffFn, GXAttnFn attnFn);
void GXSetChanAmbColor(GXChannelID chan, GXColor ambColor);
void GXSetChanMatColor(GXChannelID chan, GXColor matColor);
void GXInitLightPos(GXLightObj* obj, f32 x, f32 y, f32 z);
void GXInitLightDir(GXLightObj* obj, f32 nx, f32 ny, f32 nz);
void GXInitLightColor(GXLightObj* obj, GXColor color);
void GXInitLightDistAttn(GXLightObj* obj, f32 refDist, f32 refBrightness, GXDistAttnFn distFunc);
void GXInitLightSpot(GXLightObj* obj, f32 cutoff, GXSpotFn spotFunc);
void GXLoadLightObjImm(const GXLightObj* obj, GXLightID light);
void GXSetNumTexGens(u32 nTexGens);
void GXSetTexCoordGen(GXTexCoordID dst, GXTexGenType type, GXTexGenSrc src, u32 mtx);
void GXSetTexCoordGen2(GXTexCoordID dst, GXTexGenType type, GXTexGenSrc src,
                       u32 mtx, GXBool normalize, u32 postMtx);

// -----------------------------------------------------------------------
// Render mode (VI)
// -----------------------------------------------------------------------
void GXSetRenderMode(GXRenderModeObj* rmo);
void GXAdjustForOverscan(GXRenderModeObj* rmo, GXRenderModeObj* adj, u16 hor, u16 ver);

// -----------------------------------------------------------------------
// Fog
// -----------------------------------------------------------------------
void GXSetFog(GXFogType type, f32 startZ, f32 endZ, f32 nearZ, f32 farZ, GXColor color);
void GXSetFogRangeAdj(GXBool enable, u16 center, const GXFogAdjTable* table);
void GXInitFogAdjTable(GXFogAdjTable* table, u16 width, const f32 projmtx[4][4]);

// -----------------------------------------------------------------------
// Misc
// -----------------------------------------------------------------------
void GXSetMisc(GXMiscToken token, u32 val);
void GXSetDither(GXBool dither);
void GXSetRevSubGroup(u8 id, u8 rid);
u32  GXGetOverflowCount(void);
u32  GXResetOverflowCount(void);

#ifdef __cplusplus
}
#endif
