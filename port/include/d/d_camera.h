#pragma once
/**
 * port/include/d/d_camera.h
 * PC port stub — camera system.
 * Shadows the original 1000-line header which pulls in d_attention.h,
 * m_Do_graphic.h, d_stage large types, daHorse_c, etc.
 * Provides the minimal surface needed by f_op_camera_mng.h and
 * f_op_camera.cpp to compile.
 */
#ifndef D_D_CAMERA_H
#define D_D_CAMERA_H

#include "port/types.h"
#include "SSystem/SComponent/c_xyz.h"
#include "SSystem/SComponent/c_angle.h"
#include "SSystem/SComponent/c_sxyz.h"
#include "SSystem/SComponent/c_phase.h"
#include "f_op/f_op_actor_mng.h"

// Forward declarations — types referenced in camera_process_class members
class fopAc_ac_c;
class dAttention_c;
struct dStage_MapEvent_dt_c;

// Minimal dCamInfo_c used inside dCamera_c
class dCamInfo_c {
public:
    dCamInfo_c() {}
    cXyz   mCenter;
    cXyz   mEye;
    f32    mFovy = 45.0f;
    cSAngle mBank;
    s16    field_0x1e = 0;
};

// dCamera_c — the full GC class is ~0x9B4 bytes.
// On PC we provide a minimal stub; size mismatch is acceptable since
// STATIC_ASSERT is disabled for non-MWERKS builds.
class dCamera_c {
public:
    dCamera_c() {}
    virtual ~dCamera_c() {}

    dCamInfo_c mInfo;
    cXyz       mCenter;
    cXyz       mEye;
    f32        mFovy   = 45.0f;
    f32        mNear   = 1.0f;
    f32        mFar    = 100000.0f;
};

// Attention system forward declarations (full type in d_attention.h)
dAttention_c* dComIfGp_getAttention();

// Camera tool data — used by d_camera.h inline methods (needs stage types)
#include "d/d_stage.h"

struct dCamMapToolData {
    dCamMapToolData() { Clr(); }
    void Clr() {
        mCameraIndex = mArrowIndex = 0xFF;
        mFlags = 0;
        mpActor = nullptr;
        mPathId = 0xFF;
        mPriority = 0xFF;
    }
    stage_camera2_data_class mCamData;
    stage_arrow_data_class   mArrowData;
    int    mArrowIndex  = 0;
    s32    mCameraIndex = 0;
    fopAc_ac_c* mpActor = nullptr;
    u16    mPriority    = 0;
    u8     mFlags       = 0;
    u8     mPathId      = 0;
};

// Minimal horse forward declaration (used in dCamera_c sub-state)
class daHorse_c;

#endif /* D_D_CAMERA_H */
