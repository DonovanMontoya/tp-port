#pragma once
// port stub — J3DGD.h: GC hardware FIFO write macros → no-ops on PC
#ifndef J3DGD_H
#define J3DGD_H
#include "dolphin/gx.h"
static inline void J3DGDWrite_u8(u8)   {}
static inline void J3DGDWrite_u16(u16) {}
static inline void J3DGDWrite_u32(u32) {}
static inline void J3DGDWrite_f32(f32) {}
static inline void J3DGDWriteBPCmd(u32) {}

static inline void J3DGDSetZMode(u8, GXCompare, u8) {}
static inline void J3DGDSetBlendMode(GXBlendMode, GXBlendFactor, GXBlendFactor, GXLogicOp) {}
static inline void J3DGDSetBlendMode(GXBlendMode, GXBlendFactor, GXBlendFactor, GXLogicOp, u8) {}
static inline void J3DGDSetAlphaCompare(GXCompare, u8, GXAlphaOp, GXCompare, u8) {}
static inline void J3DGDSetFog(GXFogType, f32, f32, f32, f32, GXColor) {}
static inline void J3DGDSetFogRangeAdj(u8, u16, _GXFogAdjTable*) {}
static inline void J3DGDSetIndTexMtx(GXIndTexMtxID, f32 (*)[3], s8) {}
static inline void J3DGDSetIndTexCoordScale(GXIndTexStageID, GXIndTexScale, GXIndTexScale,
                                             GXIndTexScale, GXIndTexScale) {}
static inline void J3DGDSetIndTexOrder(u32, GXTexCoordID, GXTexMapID, GXTexCoordID, GXTexMapID,
                                        GXTexCoordID, GXTexMapID) {}
static inline void J3DGDWriteXFCmdHdr(u16, u8) {}
#endif
