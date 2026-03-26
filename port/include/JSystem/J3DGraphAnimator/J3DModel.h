#pragma once
/**
 * port/include/JSystem/J3DGraphAnimator/J3DModel.h
 * PC port stub — J3DModel and related classes with minimal stubs.
 * All rendering methods are no-ops; factory returns nullptr.
 */
#ifndef J3DMODEL_H
#define J3DMODEL_H

#include "port/types.h"
#include "SSystem/SComponent/c_xyz.h"
#include <mtx.h>

class J3DAnmTextureSRTKey;
class J3DAnmTevRegKey;

class J3DModelData {
public:
    void entryTexMtxAnimator(J3DAnmTextureSRTKey* /*anm*/) {}
    void entryTevRegAnimator(J3DAnmTevRegKey* /*anm*/) {}
    const char* getJointName(u16 /*idx*/) const { return ""; }
};

class J3DModel {
public:
    void        setBaseScale(const cXyz& /*s*/) {}
    Vec*        getBaseScale()   { return &mBaseScale; }
    Mtx&        getBaseTRMtx()  { return mBaseTRMtx; }
    J3DModelData* getModelData(){ return &mModelData; }

    // Additional methods referenced in f_op_actor.cpp
    void setAnmMtx(u16 /*joint*/, const Mtx /*m*/) {}

    Vec          mBaseScale;
    Mtx          mBaseTRMtx;
    J3DModelData mModelData;
};

// Animation key classes
class J3DAnmTextureSRTKey {
public:
    void searchUpdateMaterialID(J3DModelData* /*md*/) {}
    f32  getFrameMax() const { return 1.0f; }
    void setFrame(f32 /*f*/) {}
};

class J3DAnmTevRegKey {
public:
    void searchUpdateMaterialID(J3DModelData* /*md*/) {}
    f32  getFrameMax() const { return 1.0f; }
    void setFrame(f32 /*f*/) {}
};

#endif /* J3DMODEL_H */
