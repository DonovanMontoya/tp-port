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
#include "f_op/f_op_view.h"

class J3DDrawBuffer;
class J3DModel;
class J3DPacket;
class dKy_tevstr_c;
class dDlst_window_c;

// Forward declare types used in 2D draw list constructors
struct ResTIMG;

// Base class for all draw list objects
class dDlst_base_c {
public:
    virtual void draw() {}
    virtual ~dDlst_base_c() {}
};

class dDlst_list_c {
public:
    static void calcWipe() {}
    static void offWipe() {}
    template <typename... Args>
    int setRealShadow(Args...) { return 0; }
    template <typename... Args>
    int setSimpleShadow(Args...) { return 0; }
    template <typename... Args>
    int addRealShadow(Args...) { return 0; }
    void drawOpaListItem3d() {}
    void drawXluListItem3d() {}
    void reset() {}
    void set2DOpa(dDlst_base_c*) {}
    void set2DXlu(dDlst_base_c*) {}
    void set2DOpaTop(dDlst_base_c*) {}
    void setCopy2D(dDlst_base_c*) {}
    void setOpaListDark() {}
    void setXluListDark() {}
    void setOpaListInvisible() {}
    void setXluListInvisible() {}
    void setOpaListDarkBG() {}
    void setXluListDarkBG() {}
    void setOpaList() {}
    void setXluList() {}
    void setOpaListItem3D() {}
    void setXluListItem3D() {}
    void setOpaList3Dlast() {}
    void setXluList3Dlast() {}
    void setXluList2DScreen() {}
    void setXluListBG() {}
    void setOpaListBG() {}
    void setOpaListP0() {}
    void setXluListP0() {}
    void setOpaListMiddle() {}
    void setXluListMiddle() {}
    void setOpaListZxlu() {}
    void setXluListZxlu() {}
    void setOpaListFilter() {}
    void setXluListFilter() {}
    void setOpaListCursor() {}
    void setXluListCursor() {}
    void init() {}
    void newPeekZdata(s16, s16, u32*) {}
    void peekZdata() {}
    void setView(view_class* view) { if (view) mView = *view; }
    void setWindow(dDlst_window_c* window) { mWindow = window; }
    void setViewport(view_port_class* port) { if (port) mViewport = *port; }
    void entryZSortListZxlu(J3DPacket*, cXyz&) {}
    void entryZSortXluList(J3DPacket*, cXyz&) {}
    void drawCopy2D() {}
    void drawOpaListSky() {}
    void drawXluListSky() {}
    void drawOpaListBG() {}
    void drawOpaListDarkBG() {}
    void drawOpaListMiddle() {}
    void drawOpaList() {}
    void drawOpaListDark() {}
    void drawOpaListPacket() {}
    void drawXluListBG() {}
    void drawXluListDarkBG() {}
    void drawXluListMiddle() {}
    void drawXluList() {}
    void drawXluListDark() {}
    void drawXluListZxlu() {}
    void drawXluListFilter() {}
    void drawXluListInvisible() {}
    void drawOpaListInvisible() {}
    void drawXluList2DScreen() {}
    void drawOpaList3Dlast() {}
    void draw2DOpa() {}
    void draw2DOpaTop() {}
    void draw2DXlu() {}
    void drawOpaListFilter() {}
    void drawOpaListP0() {}
    void drawOpaListCursor() {}
    void drawXluListCursor() {}
    template <typename... Args>
    void drawShadow(Args...) {}
    template <typename... Args>
    void imageDrawShadow(Args...) {}
    template <typename... Args>
    void set3DlineMat(Args...) {}
    template <typename... Args>
    void set3DlineMatDark(Args...) {}
    J3DDrawBuffer* getOpaList() { return nullptr; }
    J3DDrawBuffer* getOpaListBG() { return nullptr; }
    J3DDrawBuffer* getOpaListDark() { return nullptr; }
    J3DDrawBuffer* getXluListBG() { return nullptr; }
    view_class* getView() { return &mView; }
    view_port_class* getViewport() { return &mViewport; }
    J3DDrawBuffer* getOpaListFilter() { return nullptr; }
    J3DDrawBuffer* getOpaListP0() { return nullptr; }
    J3DDrawBuffer* getOpaListPacket() { return nullptr; }
    void setOpaListSky() {}
    void setXluListSky() {}

private:
    view_class mView = {};
    view_port_class mViewport = {0.0f, 0.0f, 608.0f, 448.0f, 0.0f, 1.0f, {0.0f, 0.0f, 608.0f, 448.0f}};
    dDlst_window_c* mWindow = nullptr;
};

// Common draw list subclasses referenced in game headers
class dDlst_blo_c         : public dDlst_base_c {};
class dDlst_snapShot_c    : public dDlst_base_c {};
class dDlst_effectLine_c  : public dDlst_base_c {
public:
    void initRnd(int, int, int) {}
    void update(cXyz&, _GXColor&, u16, u16, u16, u16, f32, f32, f32, f32) {}
};
class dDlst_2DQuad_c      : public dDlst_base_c {};
class dDlst_2DT2_c        : public dDlst_base_c {};
class dDlst_window_c {
public:
    int getCameraID() const { return 0; }
    view_port_class* getViewPort() { return &mViewPort; }
    scissor_class* getScissor() { return &mViewPort.scissor; }
    void setViewPort(f32 xOrig, f32 yOrig, f32 width, f32 height, f32 nearZ, f32 farZ) {
        mViewPort.x_orig = xOrig;
        mViewPort.y_orig = yOrig;
        mViewPort.width = width;
        mViewPort.height = height;
        mViewPort.near_z = nearZ;
        mViewPort.far_z = farZ;
        mViewPort.scissor.x_orig = xOrig;
        mViewPort.scissor.y_orig = yOrig;
        mViewPort.scissor.width = width;
        mViewPort.scissor.height = height;
    }
    void setScissor(f32 x, f32 y, f32 width, f32 height) {
        mViewPort.scissor.x_orig = x;
        mViewPort.scissor.y_orig = y;
        mViewPort.scissor.width = width;
        mViewPort.scissor.height = height;
    }
    void setCameraID(int cameraID) { mCameraID = cameraID; }
    void setMode(u8 mode) { mMode = mode; }

private:
    int mCameraID = 0;
    u8 mMode = 0;
    u8 mPad[3] = {};
    view_port_class mViewPort = {0.0f, 0.0f, 608.0f, 448.0f, 0.0f, 1.0f, {0.0f, 0.0f, 608.0f, 448.0f}};
};

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
    static _GXTexObj* getSimpleTex() { return nullptr; }
    static void setFullDrawFlag(bool)  {}
};

// 3D line material types are defined by m_Do_ext when that source is enabled.
class mDoExt_3DlineMat_c;
class mDoExt_3DlineMatSortPacket;
class m3DLineMatSortPacket;

#endif /* D_D_DRAWLIST_H */
