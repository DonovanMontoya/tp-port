#pragma once
/**
 * port/include/JSystem/J3DGraphBase/J3DSys.h
 * PC port stub — J3D view matrix system.
 */
#ifndef J3DSYS_H
#define J3DSYS_H

#include "port/types.h"
#include <mtx.h>

class J3DSys {
public:
    J3DSys() { PSMTXIdentity(mViewMtx); PSMTXIdentity(mCurrentMtx); }
    MtxP getViewMtx() { return mViewMtx; }
    void setViewMtx(const Mtx m) { MTXCopy(m, mViewMtx); }

    Mtx  mViewMtx;
    // Static members used by J3DModelLoader
    static Mtx mCurrentMtx;
    static Vec mCurrentS;
};

extern J3DSys j3dSys;

#endif /* J3DSYS_H */
