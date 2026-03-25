#pragma once
/**
 * port/include/JSystem/J2DGraph/J2DOrthoGraph.h
 * PC port stub — 2D orthographic graphics context.
 * Inherits J2DGrafContext from libs (pure C++ template/layout, no GX calls in header).
 * Virtual methods that would call GX are stubbed.
 */
#include "JSystem/J2DGraph/J2DGrafContext.h"

class J2DOrthoGraph : public J2DGrafContext {
public:
    J2DOrthoGraph()
        : J2DGrafContext(0.0f, 0.0f, 640.0f, 480.0f)
        , mNear(-1.0f), mFar(1.0f) {}

    J2DOrthoGraph(f32 x, f32 y, f32 width, f32 height, f32 far, f32 near)
        : J2DGrafContext(x, y, width, height)
        , mNear(near), mFar(far)
    {
        setOrtho(JGeometry::TBox2<f32>(x, y, x + width, y + height), far, near);
    }

    void setOrtho(JGeometry::TBox2<f32> const& bounds, f32 far, f32 near) {
        mOrtho = bounds;
        mFar   = far;
        mNear  = near;
    }
    void setOrtho(f32 x, f32 y, f32 width, f32 height, f32 far, f32 near) {
        setOrtho(JGeometry::TBox2<f32>(x, y, x + width, y + height), far, near);
    }
    void scissorBounds(JGeometry::TBox2<f32>*, JGeometry::TBox2<f32> const*) {}

    virtual ~J2DOrthoGraph() {}
    virtual void setPort()    override {}
    virtual s32  getGrafType() const override { return 1; }
    virtual void setLookat()   override {}

    f32 getWidthPower()  { return mBounds.getWidth()  / mOrtho.getWidth(); }
    f32 getHeightPower() { return mBounds.getHeight() / mOrtho.getHeight(); }

    const JGeometry::TBox2<f32>* getOrtho() const { return &mOrtho; }

private:
    JGeometry::TBox2<f32> mOrtho;
    f32 mNear;
    f32 mFar;
};

// ---------------------------------------------------------------------------
// Global 2D draw helpers — no-ops on PC
// ---------------------------------------------------------------------------
inline void J2DDrawLine(f32, f32, f32, f32, JUtility::TColor, int) {}
inline void J2DFillBox(f32, f32, f32, f32, JUtility::TColor) {}
inline void J2DFillBox(JGeometry::TBox2<f32> const&, JUtility::TColor) {}
inline void J2DDrawFrame(f32, f32, f32, f32, JUtility::TColor, u8) {}
inline void J2DDrawFrame(JGeometry::TBox2<f32> const&, JUtility::TColor, u8) {}
