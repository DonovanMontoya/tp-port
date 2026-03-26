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

#endif /* M_DO_M_DO_MTX_H */
