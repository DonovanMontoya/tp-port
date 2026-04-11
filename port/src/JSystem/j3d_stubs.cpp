/**
 * port/src/JSystem/j3d_stubs.cpp
 * Stub definitions for J3D (GameCube 3D library) symbols that are referenced
 * by game code but whose source files use hardware GX calls we can't compile.
 *
 * Includes:
 *   - j3dDefault* constant globals (copied verbatim from J3DTevs.cpp)
 *   - Out-of-line operator= for J3DTexMtxInfo / J3DIndTexMtxInfo / J3DFogInfo
 *   - J3DDisplayListObj::beginPatch / endPatch  (no-ops — no display lists on PC)
 *   - J3DAnmTevRegKey / J3DAnmTextureSRTKey::searchUpdateMaterialID (no-ops)
 *   - JUTConsoleManager::sManager static
 *   - JUTCacheFont constructor stub
 */

#include "port/types.h"
#include <cstring>  // memcpy

// ---------------------------------------------------------------------------
// J3D default constant globals
// Must be defined before any TU pulls in J3DTevs.h constructors.
// ---------------------------------------------------------------------------
#include "JSystem/J3DGraphBase/J3DTevs.h"   // struct declarations + extern decls
#include "JSystem/J3DGraphBase/J3DMatBlock.h"

const J3DTexCoordInfo j3dDefaultTexCoordInfo[8] = {
    {GX_MTX2x4, GX_TG_TEX0, GX_IDENTITY, 0}, {GX_MTX2x4, GX_TG_TEX1, GX_IDENTITY, 0},
    {GX_MTX2x4, GX_TG_TEX2, GX_IDENTITY, 0}, {GX_MTX2x4, GX_TG_TEX3, GX_IDENTITY, 0},
    {GX_MTX2x4, GX_TG_TEX4, GX_IDENTITY, 0}, {GX_MTX2x4, GX_TG_TEX5, GX_IDENTITY, 0},
    {GX_MTX2x4, GX_TG_TEX6, GX_IDENTITY, 0}, {GX_MTX2x4, GX_TG_TEX7, GX_IDENTITY, 0},
};

const J3DTexMtxInfo j3dDefaultTexMtxInfo = {
    0x01, 0x00, 0xFF, 0xFF,
    {0.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f, 0.0f,
     0.0f, 1.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 1.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 1.0f},
};

const J3DIndTexMtxInfo j3dDefaultIndTexMtxInfo = {
    0.5f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f,
    1
};

const J3DLightInfo j3dDefaultLightInfo = {
    0.0f, 0.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0xff, 0xff, 0xff, 0xff,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
};

const J3DTevStageInfo j3dDefaultTevStageInfo = {
    0x04, 0x0A, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x05, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
};

const J3DIndTevStageInfo j3dDefaultIndTevStageInfo = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const J3DFogInfo j3dDefaultFogInfo = {
    0x00, 0x00, 0x0140, 0.0f, 0.0f, 0.1f, 10000.0f,
    {0xFF, 0xFF, 0xFF, 0x00},
    {{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}},
};

const J3DTevOrderInfo j3dDefaultTevOrderInfoNull   = {0xFF, 0xFF, 0xFF, 0x00};
const J3DIndTexOrderInfo j3dDefaultIndTexOrderNull = {0xFF, 0xFF, 0x00, 0x00};

const J3DIndTexCoordScaleInfo j3dDefaultIndTexCoordScaleInfo = {
    0x00, 0x00, 0x00, 0x00,
};

const J3DTevSwapModeInfo j3dDefaultTevSwapMode = {
    0x00, 0x00, 0x00, 0x00,
};

const J3DBlendInfo j3dDefaultBlendInfo = {
    GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP
};

const J3DColorChanInfo j3dDefaultColorChanInfo = {
    0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0xFF, 0xFF,
};

const u8  j3dDefaultTevSwapTableID = 0x1B;
const u16 j3dDefaultAlphaCmpID     = 0x00E7;
const u16 j3dDefaultZModeID        = 0x0017;

// ---------------------------------------------------------------------------
// Out-of-line operator= for structs that declare them non-trivially
// (declared in J3DStruct.h; implementations live in J3DTevs.cpp which we
//  can't compile because it contains hardware GX calls)
// ---------------------------------------------------------------------------
J3DTexMtxInfo& J3DTexMtxInfo::operator=(J3DTexMtxInfo const& o) {
    memcpy(this, &o, sizeof(J3DTexMtxInfo));
    return *this;
}

J3DIndTexMtxInfo& J3DIndTexMtxInfo::operator=(J3DIndTexMtxInfo const& o) {
    memcpy(this, &o, sizeof(J3DIndTexMtxInfo));
    return *this;
}

J3DLightInfo& J3DLightInfo::operator=(J3DLightInfo const& o) {
    memcpy(this, &o, sizeof(J3DLightInfo));
    return *this;
}

J3DFogInfo& J3DFogInfo::operator=(J3DFogInfo const& o) {
    memcpy(this, &o, sizeof(J3DFogInfo));
    return *this;
}

// ---------------------------------------------------------------------------
// J3DDisplayListObj — no display lists on PC; patch calls are no-ops
// ---------------------------------------------------------------------------
#include "JSystem/J3DGraphBase/J3DPacket.h"

void J3DDisplayListObj::beginPatch() {}
u32  J3DDisplayListObj::endPatch()   { return 0; }

// ---------------------------------------------------------------------------
// JUTConsoleManager static
// ---------------------------------------------------------------------------
#include "JSystem/JUtility/JUTConsole.h"
JUTConsoleManager* JUTConsoleManager::sManager = nullptr;

// JUTCacheFont is fully stubbed in port/include/JSystem/JUtility/JUTCacheFont.h
