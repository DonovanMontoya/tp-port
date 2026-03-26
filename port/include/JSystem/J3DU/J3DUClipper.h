#pragma once
/**
 * port/include/JSystem/J3DU/J3DUClipper.h
 * PC port stub — J3D view-frustum clipper.
 * The real class clips vertices against camera frustum planes.
 * On PC all clips return "inside" (1) — geometry drawn unconditionally.
 */
#ifndef J3DUCLIPPER_H
#define J3DUCLIPPER_H

#include "port/types.h"
#include <mtx.h>

class J3DUClipper {
public:
    J3DUClipper() { init(); }
    virtual ~J3DUClipper() {}

    void init()              {}
    void calcViewFrustum()   {}

    // Returns 1 = "inside" (no culling on PC)
    int clip(f32 const (*)[4], Vec, f32)          const { return 1; }
    int clip(f32 const (*)[4], Vec*, Vec*)         const { return 1; }

    void setFovy(f32 v)    { mFovY   = v; }
    void setAspect(f32 v)  { mAspect = v; }
    void setNear(f32 v)    { mNear   = v; }
    void setFar(f32 v)     { mFar    = v; }
    f32  getFar()          { return mFar; }

private:
    Vec  _04 = {};
    Vec  _10 = {};
    Vec  _1C = {};
    Vec  _28 = {};
    f32  mFovY   = 45.0f;
    f32  mAspect = 1.333f;
    f32  mNear   = 1.0f;
    f32  mFar    = 100000.0f;
};

#endif /* J3DUCLIPPER_H */
