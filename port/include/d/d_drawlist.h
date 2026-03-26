#pragma once
/**
 * port/include/d/d_drawlist.h
 * PC port stub — draw list classes without J2DGraph/J3D chains.
 * The original pulls in J2DPicture, J2DScreen, J3DSys → GX types.
 */
#ifndef D_D_DRAWLIST_H
#define D_D_DRAWLIST_H

#include "port/types.h"
#include "JSystem/J2DGraph/J2DPicture.h"

class J3DDrawBuffer;
class J3DModel;
class J3DPacket;
class dKy_tevstr_c;

// Forward declare types used in 2D draw list constructors
struct ResTIMG;

// Base class for all draw list objects
class dDlst_base_c {
public:
    virtual void draw() {}
    virtual ~dDlst_base_c() {}
};

// Common draw list subclasses referenced in game headers
class dDlst_blo_c         : public dDlst_base_c {};
class dDlst_snapShot_c    : public dDlst_base_c {};
class dDlst_effectLine_c  : public dDlst_base_c {};
class dDlst_2DQuad_c      : public dDlst_base_c {};
class dDlst_2DT2_c        : public dDlst_base_c {};

// 2D image display list — constructor matches the original signature
class dDlst_2D_c : public dDlst_base_c {
public:
    dDlst_2D_c() {}
    dDlst_2D_c(ResTIMG*, int, int, int, int, int) {}
    void        setAlpha(u8) {}
    J2DPicture* getPicture() { return &mPicture; }
private:
    J2DPicture mPicture;
};

struct dDlst_peekZ_c {};
struct dDlst_shadowSimple_c {};

class dDlst_shadowControl_c {
public:
    static void setSimpleTex(ResTIMG*) {}
    static void setFullDrawFlag(bool)  {}
};

// 3D line material stubs — declared to satisfy d_drawlist class members
class mDoExt_3DlineMat_c {};
class mDoExt_3DlineMatSortPacket {};
class m3DLineMatSortPacket {};

#endif /* D_D_DRAWLIST_H */
