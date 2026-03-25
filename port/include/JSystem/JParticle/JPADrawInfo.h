#pragma once
/**
 * port/include/JSystem/JParticle/JPADrawInfo.h
 * PC port stub — particle system draw info (camera/projection matrices).
 * Uses our port Mtx type; MTXCopy / C_MTXLight* replaced with no-ops.
 */
#include "port/types.h"

// ---------------------------------------------------------------------------
// Minimal Mtx type (4x3 matrix as used by GX) for PC
// We typedef it here only if the global mtx.h (port/include/mtx.h) doesn't
// already provide it.
// ---------------------------------------------------------------------------
#ifndef MTX_DEFINED
#define MTX_DEFINED
typedef f32 Mtx[3][4];
#endif

inline void PC_MTXCopy(const Mtx src, Mtx dst) {
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 4; c++)
            dst[r][c] = src[r][c];
}

inline void PC_MTXLightPerspective(Mtx m, f32 /*fovY*/, f32 /*aspect*/,
                                    f32 sx, f32 sy, f32 tx, f32 ty) {
    // Identity-ish stub — enough to not crash
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 4; c++)
            m[r][c] = 0.0f;
    m[0][0] = sx; m[1][1] = sy; m[0][3] = tx; m[1][3] = ty;
}

inline void PC_MTXLightOrtho(Mtx m, f32 /*top*/, f32 /*bottom*/, f32 /*left*/, f32 /*right*/,
                               f32 sx, f32 sy, f32 tx, f32 ty) {
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 4; c++)
            m[r][c] = 0.0f;
    m[0][0] = sx; m[1][1] = sy; m[0][3] = tx; m[1][3] = ty;
}

class JPADrawInfo {
public:
    JPADrawInfo(const Mtx camMtx, f32 fovY, f32 aspect) {
        PC_MTXCopy(camMtx, mCamMtx);
        PC_MTXLightPerspective(mPrjMtx, fovY, aspect, 0.5f, -0.5f, 0.5f, 0.5f);
    }
    JPADrawInfo(const Mtx camMtx, f32 top, f32 bottom, f32 left, f32 right) {
        PC_MTXCopy(camMtx, mCamMtx);
        PC_MTXLightOrtho(mPrjMtx, top, bottom, left, right, 0.5f, 0.5f, 0.5f, 0.5f);
    }

    Mtx mCamMtx;
    Mtx mPrjMtx;

    void getCamMtx(Mtx dst) const { PC_MTXCopy(mCamMtx, dst); }
    void getPrjMtx(Mtx dst) const { PC_MTXCopy(mPrjMtx, dst); }
    void setPrjMtx(const Mtx src) { PC_MTXCopy(src, mPrjMtx); }
};
