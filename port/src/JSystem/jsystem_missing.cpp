#ifndef CP_REG_MTXIDXA_ID
#define CP_REG_MTXIDXA_ID 0x30
#endif
#ifndef CP_REG_MTXIDXB_ID
#define CP_REG_MTXIDXB_ID 0x40
#endif
#ifndef GX_XF_REG_MATRIXINDEX0
#define GX_XF_REG_MATRIXINDEX0 0x1018
#endif
#ifndef J3DFifoLoadXFCmdHdr
#define J3DFifoLoadXFCmdHdr(...) ((void)0)
#endif

#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J3DGraphAnimator/J3DAnimation.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"
#include "JSystem/J3DGraphAnimator/J3DModelData.h"
#include "JSystem/J3DGraphBase/J3DShape.h"
#include "JSystem/J3DGraphBase/J3DSys.h"
#include "JSystem/J3DGraphBase/J3DTexture.h"
#include "JSystem/J3DGraphLoader/J3DAnmLoader.h"
#include "JSystem/J3DGraphLoader/J3DClusterLoader.h"
#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include "JSystem/JUtility/JUTProcBar.h"
#include "d/d_bg_w_kcol.h"

JUTProcBar* JUTProcBar::sManager = nullptr;
const Mtx j3dDefaultMtx = {};

extern "C" u32 GXGetTexBufferSize(u16 width, u16 height, u32, u8, u8) {
    return static_cast<u32>(width) * static_cast<u32>(height) * 4;
}

J2DGrafContext::J2DGrafContext(f32 x, f32 y, f32 width, f32 height)
    : mBounds(x, y, x + width, y + height),
      mScissorBounds(mBounds),
      mColorTL(0xffffffff),
      mColorTR(0xffffffff),
      mColorBR(0xffffffff),
      mColorBL(0xffffffff),
      mLineWidth(1),
      mPrevPos(0.0f, 0.0f),
      field_0xb0{0, 0, 0},
      mLinePart{0, 0, 0},
      mBoxPart{0, 0, 0} {}

void J2DGrafContext::place(JGeometry::TBox2<f32> const& bounds) {
    mBounds = bounds;
    mScissorBounds = bounds;
}

void J2DGrafContext::setPort() {}
void J2DGrafContext::setup2D() {}
void J2DGrafContext::setScissor() {}
void J2DGrafContext::scissor(JGeometry::TBox2<f32> const& bounds) { mScissorBounds = bounds; }
void J2DGrafContext::setColor(JUtility::TColor colorTL, JUtility::TColor colorTR,
                              JUtility::TColor colorBR, JUtility::TColor colorBL) {
    mColorTL = colorTL;
    mColorTR = colorTR;
    mColorBR = colorBR;
    mColorBL = colorBL;
}
void J2DGrafContext::setLineWidth(u8 width) { mLineWidth = width; }
void J2DGrafContext::fillBox(JGeometry::TBox2<f32> const&) {}
void J2DGrafContext::drawFrame(JGeometry::TBox2<f32> const&) {}
void J2DGrafContext::line(JGeometry::TVec2<f32> start, JGeometry::TVec2<f32> end) {
    mPrevPos = start;
    lineTo(end);
}
void J2DGrafContext::lineTo(JGeometry::TVec2<f32> pos) { mPrevPos = pos; }

void J3DMaterialAnm::initialize() {}
void J3DMaterialAnm::setMatColorAnm(int, J3DMatColorAnm*) {}
void J3DMaterialAnm::setTexNoAnm(int, J3DTexNoAnm*) {}
void J3DMaterialAnm::calc(J3DMaterial*) const {}

void J3DTexture::addResTIMG(u16 index, ResTIMG const* timg) {
    if (timg != nullptr && index < mNum) {
        mpRes[index] = *timg;
    }
}

J3DModelData* J3DModelLoaderDataBase::load(void const* i_data, u32) {
    return reinterpret_cast<J3DModelData*>(const_cast<void*>(i_data));
}

J3DModelData* J3DModelLoaderDataBase::loadBinaryDisplayList(const void* i_data, u32) {
    return reinterpret_cast<J3DModelData*>(const_cast<void*>(i_data));
}

s32 J3DModelData::newSharedDisplayList(u32) { return 0; }
void J3DModelData::makeSharedDL() {}
void J3DModelData::simpleCalcMaterial(u16, Mtx) {}

void J3DShape::addTexMtxIndexInDL(_GXAttr, u32) {}
void J3DShape::addTexMtxIndexInVcd(_GXAttr) {}

void J3DAnmTexPattern::getTexNo(u16, u16* out) const {
    if (out != nullptr) {
        *out = 0;
    }
}

J3DAnmTransform::J3DAnmTransform(s16 count, f32* scaleData, s16* rotData, f32* transData)
    : mScaleData(scaleData), mRotData(rotData), mTransData(transData), field_0x18(count),
      field_0x1a(0), field_0x1c(0), field_0x1e(0) {}

void J3DAnmTransformKey::calcTransform(f32, u16, J3DTransformInfo* out) const {
    if (out != nullptr) {
        out->mScale.x = 1.0f;
        out->mScale.y = 1.0f;
        out->mScale.z = 1.0f;
        out->mRotation.x = 0;
        out->mRotation.y = 0;
        out->mRotation.z = 0;
        out->mTranslate.x = 0.0f;
        out->mTranslate.y = 0.0f;
        out->mTranslate.z = 0.0f;
    }
}

J3DAnmBase* J3DAnmLoaderDataBase::load(void const* data, J3DAnmLoaderDataBaseFlag) {
    return reinterpret_cast<J3DAnmBase*>(const_cast<void*>(data));
}

void J3DAnmLoaderDataBase::setResource(J3DAnmBase*, void const*) {}

void* J3DClusterLoaderDataBase::load(void const* data) {
    return const_cast<void*>(data);
}

void* dBgWKCol::initKCollision(void* data) {
    return data;
}
