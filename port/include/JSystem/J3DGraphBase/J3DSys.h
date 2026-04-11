#pragma once
/**
 * PC port stub — keep a lightweight J3DSys object, but expose the fields
 * that animation code like m_Do_ext.cpp expects.
 */
#ifndef J3DSYS_H
#define J3DSYS_H

#include "port/types.h"
#include <mtx.h>

class J3DModel;
class J3DDrawBuffer;
class J3DTexture;

struct J3DTexCoordScaleInfo {
    u16 field_0x00 = 0;
    u16 field_0x02 = 0;
    u16 field_0x04 = 0;
    u16 field_0x06 = 0;
};

class J3DSys {
public:
    J3DSys() : mCurrentMtxCalc(nullptr), mModel(nullptr), mTexture(nullptr) {
        PSMTXIdentity(mViewMtx);
        PSMTXIdentity(mCurrentMtx);
        mDrawBuffer[0] = nullptr;
        mDrawBuffer[1] = nullptr;
    }

    MtxP getViewMtx() { return mViewMtx; }
    void setViewMtx(const Mtx m) { MTXCopy(m, mViewMtx); }

    void setModel(J3DModel* model) { mModel = model; }
    J3DModel* getModel() { return mModel; }
    void setCurrentMtxCalc(void* calc) { mCurrentMtxCalc = calc; }
    void setTexture(J3DTexture* texture) { mTexture = texture; }
    J3DTexture* getTexture() { return mTexture; }
    void setDrawBuffer(J3DDrawBuffer* buffer, int type) {
        if (type >= 0 && type < 2) {
            mDrawBuffer[type] = buffer;
        }
    }
    J3DDrawBuffer* getDrawBuffer(int type) {
        return (type >= 0 && type < 2) ? mDrawBuffer[type] : nullptr;
    }
    void drawInit() {}
    void reinitGX() {}
    void setVtxPos(void*) {}
    void setVtxNrm(void*) {}
    void setVtxCol(void*) {}

    Mtx mViewMtx;
    void* mCurrentMtxCalc;
    J3DModel* mModel;
    J3DDrawBuffer* mDrawBuffer[2];
    J3DTexture* mTexture;

    static Mtx mCurrentMtx;
    static Vec mCurrentS;
    static Vec mParentS;
    static J3DTexCoordScaleInfo sTexCoordScaleTable[8];
};

extern u32 j3dDefaultViewNo;
extern J3DSys j3dSys;

#endif /* J3DSYS_H */
