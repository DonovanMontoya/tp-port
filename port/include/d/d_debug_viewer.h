#pragma once
/**
 * port/include/d/d_debug_viewer.h
 * PC port stub — debug drawing functions.
 * All debug visualisations are no-ops on PC (no GX wireframe renderer).
 */
#ifndef D_DEBUG_VIEWER_H
#define D_DEBUG_VIEWER_H

#include "port/types.h"
#include "SSystem/SComponent/c_xyz.h"
#include "SSystem/SComponent/c_sxyz.h"
#include <mtx.h>

// GXColor is u8 r,g,b,a — use dolphin/gx/GXStruct.h definition
#include <dolphin/gx/GXStruct.h>

class J3DPacket;

// All functions are no-ops on PC
inline J3DPacket* dDbVw_setDrawPacketList(J3DPacket*, int) { return nullptr; }
inline void dDbVw_deleteDrawPacketList() {}

inline void dDbVw_drawCube8p(int, cXyz*, const GXColor&) {}
inline void dDbVw_drawCube(int, cXyz&, cXyz&, csXyz&, const GXColor&) {}
inline void dDbVw_drawTriangle(int, cXyz*, const GXColor&, u8) {}
inline void dDbVw_drawQuad(int, cXyz*, const GXColor&, u8) {}
inline void dDbVw_drawQuad(int, cXyz*, cXyz*, cXyz*, s16, const GXColor&, u8) {}
inline void dDbVw_drawLine(int, cXyz&, cXyz&, const GXColor&, u8, u8) {}
inline void dDbVw_drawArrow(int, cXyz&, cXyz&, const GXColor&, u8, u8) {}
inline void dDbVw_drawPoint(int, cXyz&, const GXColor&, u8, u8) {}
inline void dDbVw_drawCircle(int, cXyz&, f32, const GXColor&, u8, u8) {}
inline void dDbVw_drawSphere(int, cXyz&, f32, const GXColor&, u8) {}
inline void dDbVw_drawCylinder(int, cXyz&, f32, f32, const GXColor&, u8) {}
inline void dDbVw_drawCylinderM(int, Mtx, const GXColor&, u8) {}
inline void dDbVw_drawCube8pXlu(cXyz*, const GXColor&) {}

// Convenience wrappers (from original header)
inline void dDbVw_drawCube8pOpa(cXyz* pts, const GXColor& col) { dDbVw_drawCube8p(0, pts, col); }
inline void dDbVw_drawCubeOpa(cXyz& p, cXyz& s, csXyz& a, const GXColor& c) { dDbVw_drawCube(0, p, s, a, c); }
inline void dDbVw_drawCubeXlu(cXyz& p, cXyz& s, csXyz& a, const GXColor& c) { dDbVw_drawCube(1, p, s, a, c); }
inline void dDbVw_drawSphereOpa(cXyz& p, f32 r, const GXColor& c, u8 cl) { dDbVw_drawSphere(0, p, r, c, cl); }
inline void dDbVw_drawSphereXlu(cXyz& p, f32 r, const GXColor& c, u8 cl) { dDbVw_drawSphere(1, p, r, c, cl); }
inline void dDbVw_drawCylinderOpa(cXyz& p, f32 r, f32 h, const GXColor& c, u8 cl) { dDbVw_drawCylinder(0, p, r, h, c, cl); }
inline void dDbVw_drawCylinderXlu(cXyz& p, f32 r, f32 h, const GXColor& c, u8 cl) { dDbVw_drawCylinder(1, p, r, h, c, cl); }

#endif /* D_DEBUG_VIEWER_H */
