#pragma once
/**
 * port/include/m_Do/m_Do_mtx.h
 * PC port stub — matrix helpers.
 * Shadows the original which pulls in GC mtx.h types (already available as port stubs).
 */
#ifndef M_DO_M_DO_MTX_H
#define M_DO_M_DO_MTX_H

#include "port/types.h"
#include <mtx.h>
#include "SSystem/SComponent/c_xyz.h"
#include "SSystem/SComponent/c_sxyz.h"

extern u8 g_printCurrentHeapDebug;
extern u8 g_printOtherHeapDebug;

// Common matrix operations — delegates to the GC compat layer
void mDoMtx_XYZrotS(Mtx, s16, s16, s16);
void mDoMtx_XYZrotM(Mtx, s16, s16, s16);
void mDoMtx_ZXYrotS(Mtx, s16, s16, s16);
void mDoMtx_ZXYrotM(Mtx, s16, s16, s16);

inline void mDoMtx_copy(const Mtx src, Mtx dst) { PSMTXCopy(src, dst); }
inline void cMtx_copy(const Mtx src, Mtx dst)   { PSMTXCopy(src, dst); }
inline void mDoMtx_trans(Mtx m, f32 x, f32 y, f32 z) { MTXTrans(m, x, y, z); }
inline void mDoMtx_YrotS(Mtx m, s16 y) { (void)m; (void)y; }
inline void mDoMtx_XrotS(Mtx m, s16 x) { (void)m; (void)x; }
inline void mDoMtx_ZrotS(Mtx m, s16 z) { (void)m; (void)z; }
inline void mDoMtx_XrotM(Mtx m, s16 x) { (void)m; (void)x; }
inline void mDoMtx_YrotM(Mtx m, s16 y) { (void)m; (void)y; }
inline void mDoMtx_ZrotM(Mtx m, s16 z) { (void)m; (void)z; }

// Matrix operations used by f_op_actor_mng and other code
inline void cMtx_concat(const Mtx a, const Mtx b, Mtx c)          { MTXConcat(a, b, c); }
inline void cMtx_YrotS(Mtx m, s16 y)                               { (void)m; (void)y; }
inline void cMtx_multVec(const Mtx m, const Vec* src, Vec* dst)    { MTXMultVec(m, src, dst); }
inline void cMtx_multVecArray(const Mtx m, const Vec* src, Vec* dst, u32 n) {
    MTXMultVecArray(m, src, dst, n);
}

// Stack matrix class — used by fopAcM_setEffectMtx and many actors
class mDoMtx_stack_c {
public:
    static Mtx  now;
    static Mtx  buffer[16];
    static Mtx* next;
    static Mtx* end;

    mDoMtx_stack_c() { next = buffer; end = buffer + 16; }
    ~mDoMtx_stack_c() {}

    static bool push() {
        if (next >= end) return false;
        PSMTXCopy(now, *next++); return true;
    }
    static bool pop() {
        if (next <= buffer) return false;
        PSMTXCopy(*--next, now); return true;
    }
    static MtxP get() { return now; }

    static void copy(const Mtx m)    { PSMTXCopy(m, now); }
    static void concat(const Mtx m)  { MTXConcat(now, m, now); }
    static void revConcat(const Mtx m) { MTXConcat(m, now, now); }
    static void identity()           { PSMTXIdentity(now); }

    static void transS(f32 x, f32 y, f32 z)   { MTXTrans(now, x, y, z); }
    static void transS(cXyz const& xyz)        { MTXTrans(now, xyz.x, xyz.y, xyz.z); }
    static void transM(f32 x, f32 y, f32 z)   { Mtx t; MTXTrans(t, x, y, z); MTXConcat(now, t, now); }
    static void transM(cXyz const& xyz)        { transM(xyz.x, xyz.y, xyz.z); }
    static void scaleS(f32 x, f32 y, f32 z)   { MTXScale(now, x, y, z); }
    static void scaleS(cXyz const& xyz)        { MTXScale(now, xyz.x, xyz.y, xyz.z); }
    static void scaleM(f32 x, f32 y, f32 z)   { Mtx s; MTXScale(s, x, y, z); MTXConcat(now, s, now); }
    static void scaleM(cXyz const& xyz)        { scaleM(xyz.x, xyz.y, xyz.z); }

    static void XYZrotS(s16 x, s16 y, s16 z)  { mDoMtx_XYZrotS(now, x, y, z); }
    static void XYZrotM(s16 x, s16 y, s16 z)  { mDoMtx_XYZrotM(now, x, y, z); }
    static void ZXYrotS(s16 x, s16 y, s16 z)  { mDoMtx_ZXYrotS(now, x, y, z); }
    static void ZXYrotM(s16 x, s16 y, s16 z)  { mDoMtx_ZXYrotM(now, x, y, z); }
    static void XYZrotS(csXyz const& xyz)      { mDoMtx_XYZrotS(now, xyz.x, xyz.y, xyz.z); }
    static void XYZrotM(csXyz const& xyz)      { mDoMtx_XYZrotM(now, xyz.x, xyz.y, xyz.z); }
    static void ZXYrotS(csXyz const& xyz)      { mDoMtx_ZXYrotS(now, xyz.x, xyz.y, xyz.z); }
    static void ZXYrotM(csXyz const& xyz)      { mDoMtx_ZXYrotM(now, xyz.x, xyz.y, xyz.z); }
    static void YrotS(s16 y)  { mDoMtx_YrotS(now, y); }
    static void YrotM(s16 y)  { mDoMtx_YrotM(now, y); }
    static void XrotS(s16 x)  { mDoMtx_XrotS(now, x); }
    static void XrotM(s16 x)  { mDoMtx_XrotM(now, x); }
    static void ZrotS(s16 z)  { mDoMtx_ZrotS(now, z); }
    static void ZrotM(s16 z)  { mDoMtx_ZrotM(now, z); }

    static void multVec(const Vec* a, Vec* b)  { PSMTXMultVec(now, a, b); }
    static void multVecSR(const Vec* a, Vec* b){ MTXMultVecSR(now, a, b); }
    static void multVecZero(Vec* v)            { Vec z = {0,0,0}; PSMTXMultVec(now, &z, v); }
    static void multVecArray(const Vec* src, Vec* dst, u32 n) { PSMTXMultVecArray(now, src, dst, n); }

    static void inverse()             { PSMTXInverse(now, now); }
};

#endif /* M_DO_M_DO_MTX_H */
