#pragma once
/**
 * port/include/m_Do/m_Do_ext.h
 * PC port stub — m_Do extension helpers.
 * Shadows the original which pulls in J3DAnimation, Z2SoundObject, etc.
 * f_op_overlap.cpp includes this header.
 */
#ifndef M_DO_M_DO_EXT_H
#define M_DO_M_DO_EXT_H

#include "port/types.h"
#include "JSystem/JKernel/JKRHeap.h"

class JKRArchive;
class JKRAssertHeap;
class JKRExpHeap;
class JKRSolidHeap;
struct ResTIMG;
class J3DModel;
class J3DModelData;

// J3DTransformInfo — joint transform result passed to morph callbacks
struct J3DTransformInfo { char _pad[0x40]; };

// Animation stub base
class mDoExt_baseAnm {
public:
    void* mpAnmBase = nullptr;
    f32   mFrame    = 0.0f;
    f32   mSpeed    = 1.0f;
    bool  mIsLoop   = false;
};

// BCK — bone colour key animation
class mDoExt_bckAnm : public mDoExt_baseAnm {
public:
    void* mAnm = nullptr;
    mDoExt_bckAnm() {}
};

// BPK — bone path key
class mDoExt_bpkAnm : public mDoExt_baseAnm {
public:
    mDoExt_bpkAnm() {}
};

// BRK — register key
class mDoExt_brkAnm : public mDoExt_baseAnm {
public:
    mDoExt_brkAnm() {}
};

// BTK — texture key
class mDoExt_btkAnm : public mDoExt_baseAnm {
public:
    mDoExt_btkAnm() {}
};

// Morph callback base classes
class mDoExt_McaMorfCallBack1_c {
public:
    virtual int execute(u16, J3DTransformInfo*) { return 1; }
    virtual ~mDoExt_McaMorfCallBack1_c() {}
};
class mDoExt_McaMorfCallBack2_c {
public:
    virtual int execute(u16, J3DTransformInfo*) { return 1; }
    virtual ~mDoExt_McaMorfCallBack2_c() {}
};

namespace mDoExt {

inline JKRExpHeap* getHeap(int /*tag*/) { return nullptr; }

} // namespace mDoExt

// J3D model factory — returns nullptr on PC (no GX renderer)
inline J3DModel* mDoExt_J3DModel__create(J3DModelData* /*md*/, u32 /*flags*/, u32 /*param*/) {
    return nullptr;
}

// Display list update — no-op on PC
inline void mDoExt_modelUpdateDL(J3DModel* /*model*/) {}

#endif /* M_DO_M_DO_EXT_H */
