#pragma once
/**
 * port/include/JSystem/JMath/JMath.h
 * PC port stub — JMath fast math operations.
 */
#ifndef JMATH_H
#define JMATH_H

#include "port/types.h"
#include <cmath>
#include <mtx.h>

// Fast inverse square root estimate
#undef __frsqrtes
#undef __frsqrte
inline float __frsqrtes(double f) { return (f > 0.0) ? (float)(1.0 / sqrt(f)) : 0.0f; }
inline float __frsqrte(float f)   { return (f > 0.0f) ? 1.0f / sqrtf(f) : 0.0f; }

inline int  JMAAbs(int x)   { return x < 0 ? -x : x; }
inline f32  JMAAbs(f32 x)   { return fabsf(x); }
inline f32  JMAFastReciprocal(f32 v) { return v != 0.0f ? 1.0f / v : 0.0f; }
inline f32  JMAFastSqrt(f32 x) { return x > 0.0f ? sqrtf(x) : x; }
inline f32  JMAHermiteInterpolation(f32 p1, f32 p2, f32 p3, f32 p4, f32 p5, f32 p6, f32 p7) {
    float t = (p1 - p2) / (p5 - p2);
    float t2 = t * t;
    float t3 = t2 * t;
    return p3 * (2*t3 - 3*t2 + 1) + p6 * (-2*t3 + 3*t2) + p4 * (t3 - 2*t2 + t) + p7 * (t3 - t2);
}

void JMAMTXApplyScale(const Mtx, Mtx, f32, f32, f32);
void JMAEulerToQuat(s16, s16, s16, Quaternion*);
void JMAQuatLerp(const Quaternion*, const Quaternion*, f32, Quaternion*);
void JMAFastVECNormalize(const Vec* src, Vec* dst);
void JMAVECScaleAdd(const Vec* vec1, const Vec* vec2, Vec* dst, f32 scale);

namespace JMath {
template <typename T>
inline T fastSqrt(T v) { return JMAFastSqrt(v); }
inline f32 fastReciprocal(f32 v) { return JMAFastReciprocal(v); }
}

namespace JMathInlineVEC {
inline void C_VECAdd(const Vec* a, const Vec* b, Vec* ab) {
    ab->x = a->x + b->x;
    ab->y = a->y + b->y;
    ab->z = a->z + b->z;
}

inline void C_VECSubtract(const Vec* a, const Vec* b, Vec* ab) {
    ab->x = a->x - b->x;
    ab->y = a->y - b->y;
    ab->z = a->z - b->z;
}

inline f32 C_VECSquareMag(const Vec* v) {
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

inline f32 C_VECDotProduct(const Vec* a, const Vec* b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}
}

#endif /* JMATH_H */
