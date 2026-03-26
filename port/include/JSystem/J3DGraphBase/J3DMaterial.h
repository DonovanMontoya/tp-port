#pragma once
/**
 * port/include/JSystem/J3DGraphBase/J3DMaterial.h
 * PC port stub — J3D material system.
 * The full class requires J3DMatBlock, J3DPacket, J3DShape (GX-heavy chains).
 * Only J3DMaterial* (pointer) is used in f_op_actor_mng.cpp, so we just need
 * the class to be declared with the methods actually called.
 */
#ifndef J3DMATERIAL_H
#define J3DMATERIAL_H

#include "port/types.h"
#include <mtx.h>

// Forward declarations — types used in J3DMaterial interface
struct J3DDisplayListObj;
class J3DJoint;
class J3DMaterialAnm;
class J3DShape;

// J3DTexMtxInfo — holds projection/SRT info for a texture matrix
struct J3DTexMtxInfo {
    u8  mProjection = 0;
    u8  mInfo       = 0;
    u8  _pad0       = 0;
    u8  _pad1       = 0;
    Vec mCenter     = {};
    // SRT + effect matrix omitted (not accessed on PC)
    void setEffectMtx(Mtx) {}
};

// J3DTexMtx — texture matrix wrapper; no GX load on PC
class J3DTexMtx {
public:
    J3DTexMtx() {}
    J3DTexMtxInfo& getTexMtxInfo() { return mTexMtxInfo; }
    void setEffectMtx(Mtx m) { mTexMtxInfo.setEffectMtx(m); }
private:
    J3DTexMtxInfo mTexMtxInfo;
};

// Minimal block type stubs (sizes don't matter on PC — STATIC_ASSERT disabled)
class J3DColorBlock   { public: virtual ~J3DColorBlock()   {} };
class J3DTexGenBlock  { public: virtual ~J3DTexGenBlock()  {} };
class J3DTevBlock     { public: virtual ~J3DTevBlock()     {} };
class J3DIndBlock     { public: virtual ~J3DIndBlock()     {} };
class J3DPEBlock      { public: virtual ~J3DPEBlock()      {} };

class J3DMaterial {
public:
    // Static factory methods (return nullptr — no GX renderer on PC)
    static J3DColorBlock*  createColorBlock(u32)     { return nullptr; }
    static J3DTexGenBlock* createTexGenBlock(u32)    { return nullptr; }
    static J3DTevBlock*    createTevBlock(int)       { return nullptr; }
    static J3DIndBlock*    createIndBlock(int)       { return nullptr; }
    static J3DPEBlock*     createPEBlock(u32, u32)   { return nullptr; }
    static u32 calcSizeColorBlock(u32)               { return 0; }
    static u32 calcSizeTexGenBlock(u32)              { return 0; }
    static u32 calcSizeTevBlock(int)                 { return 0; }
    static u32 calcSizeIndBlock(int)                 { return 0; }
    static u32 calcSizePEBlock(u32, u32)             { return 0; }

    void initialize()                                {}
    u32  countDLSize()                               { return 0; }
    void makeDisplayList_private(J3DDisplayListObj*) {}

    const char*   getName()      const { return ""; }
    u16           getIndex()     const { return 0; }
    J3DShape*     getShape()     const { return nullptr; }
    J3DMaterialAnm* getMaterialAnm() const { return nullptr; }
    J3DTexMtx*    getTexMtx(u32) const { return nullptr; }
};

#endif /* J3DMATERIAL_H */
