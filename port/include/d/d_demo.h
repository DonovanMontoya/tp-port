#pragma once
/**
 * port/include/d/d_demo.h
 * PC port stub — demo/cutscene system types.
 * Shadows the original which pulls in JStudio → JGeometry chain.
 * On PC cutscenes are not played; all stubs are no-ops.
 */

#ifndef D_D_DEMO_H
#define D_D_DEMO_H

#include "JSystem/JGadget/binary.h"
#include "JSystem/JStudio/JStudio/stb.h"
#include "port/types.h"
#include "SSystem/SComponent/c_sxyz.h"
#include "SSystem/SComponent/c_xyz.h"

#ifndef ASSERT
#define ASSERT(expr) ((void)0)
#endif

using JGadget::binary::TValueIterator_raw;

class J3DModel;

struct dDemo_prm_data {
    u8 raw[0x10];
};

class dDemo_prm_c {
public:
    dDemo_prm_c() : mData(nullptr) {}
    dDemo_prm_data* getData() { return mData; }

    u32 field_0x0 = 0;
    dDemo_prm_data* mData;
};

class dDemo_actor_c {
public:
    enum {
        ENABLE_UNK_e = 1,
        ENABLE_TRANS_e = (1 << 1),
        ENABLE_SCALE_e = (1 << 2),
        ENABLE_ROTATE_e = (1 << 3),
        ENABLE_SHAPE_e = (1 << 4),
        ENABLE_ANM_e = (1 << 5),
        ENABLE_ANM_FRAME_e = (1 << 6),
        ENABLE_ANM_TRANSITION_e = (1 << 7),
        ENABLE_TEX_ANM = (1 << 8),
        ENABLE_TEX_ANM_FRAME_e = (1 << 9),
    };

    void setActor(void* /*p*/) {}
    void setModel(J3DModel* /*p*/) {}
    bool checkEnable(u16 /*flag*/) { return false; }
    dDemo_prm_c* getPrm() { return nullptr; }
    int getAnmId() { return 0; }
    f32 getPrm_Morf() { return 0.0f; }
    cXyz& getTrans() { return mTrans; }
    cXyz& getScale() { return mScale; }
    csXyz& getRatate() { return mRotate; }
    int getDemoIDData(int*, int*, int*, u16*, u8*) { return 0; }
    void setAnmFrameMax(f32 max) { mAnmFrameMax = max; }
    f32 getAnmFrame() const { return mAnmFrame; }
    f32 getAnmTransition() const { return 0.0f; }
    u32 getShapeId() const { return 0; }

private:
    cXyz mTrans;
    cXyz mScale;
    csXyz mRotate;
    f32 mAnmFrame = 0.0f;
    f32 mAnmFrameMax = 0.0f;
};

class dDemo_camera_c {
public:
    enum {
        ENABLE_PROJ_NEAR_e = (1 << 0),
        ENABLE_PROJ_FAR_e = (1 << 1),
        ENABLE_PROJ_FOVY_e = (1 << 2),
        ENABLE_PROJ_ASPECT_e = (1 << 3),
        ENABLE_VIEW_POS_e = (1 << 4),
        ENABLE_VIEW_UP_VEC_e = (1 << 5),
        ENABLE_VIEW_TARG_POS_e = (1 << 6),
        ENABLE_VIEW_ROLL_e = (1 << 7),
    };

    bool checkEnable(u8 /*flag*/) { return false; }
    cXyz& getTarget() { return mTarget; }
    cXyz& getTrans() { return mTrans; }
    cXyz& getUp() { return mUp; }
    f32 getRoll() const { return 0.0f; }
    f32 getFovy() const { return 45.0f; }

private:
    cXyz mTarget;
    cXyz mTrans;
    cXyz mUp = cXyz(0.0f, 1.0f, 0.0f);
};

class dDemo_c {
public:
    static void create() {}
    static dDemo_actor_c* getActor(u8 /*id*/) { return nullptr; }
    static dDemo_camera_c* getCamera() { return nullptr; }
    static bool isStatus(u32 /*status*/) { return false; }
    static void onStatus(u32 /*status*/) {}
    static void offStatus(u32 /*status*/) {}
    static void reset() {}
    static void update() {}
    static void remove() {}
    static u32  getFrame() { return 0; }
    static void setBranchId(u16 /*branch*/, s16 id) { m_branchId = id; }
    static s16 getBranchId() { return m_branchId; }

private:
    inline static s16 m_branchId = -1;
};

#endif /* D_D_DEMO_H */
