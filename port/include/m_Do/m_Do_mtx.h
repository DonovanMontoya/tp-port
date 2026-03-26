#pragma once
#ifndef M_DO_M_DO_MTX_H
#define M_DO_M_DO_MTX_H

#include "SSystem/SComponent/c_sxyz.h"
#include "SSystem/SComponent/c_xyz.h"
#include <dolphin/mtx.h>
#include <mtx.h>

extern u8 g_printCurrentHeapDebug;
extern u8 g_printOtherHeapDebug;

void mDoMtx_XYZrotS(Mtx, s16, s16, s16);
void mDoMtx_XYZrotM(Mtx, s16, s16, s16);
void mDoMtx_ZXYrotS(Mtx, s16, s16, s16);
void mDoMtx_ZXYrotM(Mtx, s16, s16, s16);
void mDoMtx_ZrotS(Mtx, s16);
void mDoMtx_YrotS(Mtx, s16);
void mDoMtx_XrotS(Mtx, s16);
void mDoMtx_XrotM(Mtx, s16);
void mDoMtx_YrotM(Mtx, s16);
void mDoMtx_ZrotM(Mtx, s16);
void mDoMtx_MtxToRot(CMtxP, csXyz*);
void mDoMtx_lookAt(Mtx, Vec const*, Vec const*, s16);
void mDoMtx_lookAt(Mtx, Vec const*, Vec const*, Vec const*, s16);
void mDoMtx_concatProjView(const Mtx, const Mtx, Mtx);
bool mDoMtx_inverseTranspose(const Mtx, Mtx);
void mDoMtx_QuatConcat(Quaternion const*, Quaternion const*, Quaternion*);

inline void mDoMtx_multVecSR(const Mtx m, const Vec* src, Vec* dst) {
    MTXMultVecSR(m, src, dst);
}

inline void mDoMtx_concat(const Mtx a, const Mtx b, Mtx c) {
    PSMTXConcat(a, b, c);
}

inline void cMtx_concat(const Mtx a, const Mtx b, Mtx ab) {
    mDoMtx_concat(a, b, ab);
}

inline void mDoMtx_multVec(CMtxP m, const Vec* src, Vec* dst) {
    PSMTXMultVec(m, src, dst);
}

inline void mDoMtx_multVecArray(const Mtx m, const Vec* src, Vec* dst, u32 count) {
    PSMTXMultVecArray(m, src, dst, count);
}

inline void mDoMtx_copy(const Mtx src, Mtx dst) {
    PSMTXCopy(src, dst);
}

inline void cMtx_copy(const Mtx src, Mtx dst) {
    mDoMtx_copy(src, dst);
}

inline void mDoMtx_trans(Mtx m, f32 x, f32 y, f32 z) {
    MTXTrans(m, x, y, z);
}

inline void cMtx_trans(Mtx m, f32 x, f32 y, f32 z) {
    mDoMtx_trans(m, x, y, z);
}

inline void mDoMtx_identity(Mtx m) {
    PSMTXIdentity(m);
}

inline void cMtx_identity(Mtx m) {
    mDoMtx_identity(m);
}

inline void mDoMtx_inverse(const Mtx a, Mtx b) {
    MTXInverse(a, b);
}

inline void cMtx_inverse(const Mtx a, Mtx b) {
    mDoMtx_inverse(a, b);
}

inline void mDoMtx_scale(Mtx m, f32 x, f32 y, f32 z) {
    MTXScale(m, x, y, z);
}

inline void cMtx_scale(Mtx m, f32 x, f32 y, f32 z) {
    mDoMtx_scale(m, x, y, z);
}

inline void mDoMtx_quat(Mtx m, const Quaternion* q) {
    MTXQuat(m, q);
}

inline MtxP cMtx_getIdentity();

inline void cMtx_XrotM(Mtx m, s16 x) { mDoMtx_XrotM(m, x); }
inline void cMtx_YrotM(Mtx m, s16 y) { mDoMtx_YrotM(m, y); }
inline void cMtx_ZrotM(Mtx m, s16 z) { mDoMtx_ZrotM(m, z); }
inline void cMtx_XrotS(Mtx m, s16 x) { mDoMtx_XrotS(m, x); }
inline void cMtx_YrotS(Mtx m, s16 y) { mDoMtx_YrotS(m, y); }
inline void cMtx_ZrotS(Mtx m, s16 z) { mDoMtx_ZrotS(m, z); }
inline void cMtx_multVec(const Mtx m, const Vec* src, Vec* dst) { mDoMtx_multVec(m, src, dst); }
inline void cMtx_multVecSR(const Mtx m, const Vec* src, Vec* dst) { mDoMtx_multVecSR(m, src, dst); }
inline void cMtx_multVecArray(const Mtx m, const Vec* src, Vec* dst, u32 count) { mDoMtx_multVecArray(m, src, dst, count); }
inline void cMtx_lookAt(Mtx m, const Vec* a, const Vec* b, s16 c) { mDoMtx_lookAt(m, a, b, c); }
inline void cMtx_lookAt(Mtx m, const Vec* a, const Vec* b, const Vec* c, s16 d) { mDoMtx_lookAt(m, a, b, c, d); }
inline void cMtx_inverseTranspose(const Mtx a, Mtx b) { mDoMtx_inverseTranspose(a, b); }
inline void cMtx_concatProjView(const Mtx a, const Mtx b, Mtx c) { mDoMtx_concatProjView(a, b, c); }

inline void mDoMtx_multVecZero(CMtxP m, Vec* dst) {
    dst->x = m[0][3];
    dst->y = m[1][3];
    dst->z = m[2][3];
}

inline void mDoMtx_quatMultiply(const Quaternion* a, const Quaternion* b, Quaternion* ab) {
    QUATMultiply(a, b, ab);
}

inline void mDoMtx_quatSlerp(const Quaternion* a, const Quaternion* b, Quaternion* ab, f32 t) {
    C_QUATSlerp(a, b, ab, t);
}

inline void mDoMtx_quatRotAxisRad(Quaternion* q, const Vec* axis, f32 rad) {
    C_QUATRotAxisRad(q, axis, rad);
}

class mDoMtx_stack_c {
public:
    mDoMtx_stack_c() {
        next = buffer;
        end = buffer + 16;
    }

    static bool push();
    static bool pop();
    static void transS(cXyz const&);
    static void transM(cXyz const&);
    static void transM(f32, f32, f32);
    static void scaleS(cXyz const&);
    static void scaleM(cXyz const&);
    static void scaleM(f32, f32, f32);
    static void XYZrotS(csXyz const&);
    static void XYZrotM(csXyz const&);
    static void ZXYrotS(csXyz const&);
    static void ZXYrotM(csXyz const&);
    static void quatM(Quaternion const*);
    ~mDoMtx_stack_c() {}

    static MtxP get() { return now; }
    static void transS(f32 x, f32 y, f32 z) { MTXTrans(now, x, y, z); }
    static void scaleS(f32 x, f32 y, f32 z) { MTXScale(now, x, y, z); }
    static void multVec(const Vec* a, Vec* b) { PSMTXMultVec(now, a, b); }
    static void multVecSR(const Vec* a, Vec* b) { MTXMultVecSR(now, a, b); }
    static void multVecZero(Vec* v) { mDoMtx_multVecZero(now, v); }
    static void multVecArray(const Vec* src, Vec* dst, u32 count) { PSMTXMultVecArray(now, src, dst, count); }
    static void XYZrotS(s16 x, s16 y, s16 z) { mDoMtx_XYZrotS(now, x, y, z); }
    static void XYZrotM(s16 x, s16 y, s16 z) { mDoMtx_XYZrotM(now, x, y, z); }
    static void ZXYrotS(s16 x, s16 y, s16 z) { mDoMtx_ZXYrotS(now, x, y, z); }
    static void ZXYrotM(s16 x, s16 y, s16 z) { mDoMtx_ZXYrotM(now, x, y, z); }
    static void YrotM(s16 y) { mDoMtx_YrotM(now, y); }
    static void YrotS(s16 y) { mDoMtx_YrotS(now, y); }
    static void XrotS(s16 x) { mDoMtx_XrotS(now, x); }
    static void XrotM(s16 x) { mDoMtx_XrotM(now, x); }
    static void ZrotS(s16 z) { mDoMtx_ZrotS(now, z); }
    static void ZrotM(s16 z) { mDoMtx_ZrotM(now, z); }
    static void inverse() { PSMTXInverse(now, now); }
    static void inverseTranspose() { mDoMtx_inverseTranspose(now, now); }
    static void concat(const Mtx m) { MTXConcat(now, m, now); }
    static void revConcat(const Mtx m) { MTXConcat(m, now, now); }
    static void copy(const Mtx m) { PSMTXCopy(m, now); }
    static void rotAxisRadS(const Vec* axis, f32 rad) { MTXRotAxisRad(now, axis, rad); }
    static void identity() { PSMTXIdentity(now); }
    static void quatS(const Quaternion* quat) { MTXQuat(now, quat); }

    static Mtx now;
    static Mtx buffer[16];
    static Mtx* next;
    static Mtx* end;
};

extern Mtx g_mDoMtx_identity;

inline MtxP mDoMtx_getIdentity() {
    return g_mDoMtx_identity;
}

inline MtxP cMtx_getIdentity() {
    return mDoMtx_getIdentity();
}

class mDoMtx_quatStack_c {
public:
    mDoMtx_quatStack_c() {
        field_0x0 = &field_0x4;
        field_0x114 = field_0x14;
        field_0x118 = &field_0x114;
    }
    ~mDoMtx_quatStack_c() {}

    Quaternion* field_0x0;
    Quaternion field_0x4;
    Quaternion field_0x14[16];
    Quaternion* field_0x114;
    Quaternion** field_0x118;
};

#endif
