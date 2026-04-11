#ifndef CP_REG_MTXIDXA_ID
#define CP_REG_MTXIDXA_ID 0x30
#endif
#ifndef CP_REG_MTXIDXB_ID
#define CP_REG_MTXIDXB_ID 0x40
#endif
#ifndef GX_XF_REG_MATRIXINDEX0
#define GX_XF_REG_MATRIXINDEX0 0x1018
#endif

#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J3DGraphAnimator/J3DAnimation.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"
#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "JSystem/J3DGraphAnimator/J3DModelData.h"
#include "JSystem/J3DGraphBase/J3DShape.h"
#include "JSystem/J3DGraphBase/J3DSys.h"
#include "JSystem/J3DGraphBase/J3DTexture.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J2DGraph/J2DManage.h"
#include "JSystem/J3DGraphLoader/J3DAnmLoader.h"
#include "JSystem/J3DGraphLoader/J3DClusterLoader.h"
#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAExTexShape.h"
#include "JSystem/JParticle/JPAResourceManager.h"
#include "JSystem/JParticle/JPADynamicsBlock.h"
#include "JSystem/JParticle/JPAEmitterManager.h"
#include "JSystem/JParticle/JPAResource.h"
#include "JSystem/JParticle/JPATexture.h"
#include "JSystem/JUtility/JUTPalette.h"
#include "JSystem/JUtility/JUTProcBar.h"
#include "JSystem/JUtility/JUTResource.h"
#include "d/d_bg_w.h"
#include "d/d_bg_w_kcol.h"

JUTProcBar* JUTProcBar::sManager = nullptr;
J2DDataManage* J2DScreen::mDataManage = nullptr;
const Mtx j3dDefaultMtx = {};

extern "C" u32 GXGetTexBufferSize(u16 width, u16 height, u32, u8, u8) {
    return static_cast<u32>(width) * static_cast<u32>(height) * 4;
}

extern "C" void GXEnableTexOffsets(GXTexCoordID, GXBool, GXBool) {}
extern "C" void GXInvalidateVtxCache(void) {}
extern "C" void GXSetTevDirect(GXTevStageID) {}
extern "C" void GXSetIndTexMtx(GXIndTexMtxID, const f32[2][3], s8) {}

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

void J2DScreen::animation() {}

void* J2DDataManage::get(char const*) {
    return nullptr;
}

void* J2DDataManage::get(JSUInputStream*) {
    return nullptr;
}

char* J2DResReference::getResReference(u16) const {
    return nullptr;
}

char* J2DResReference::getName(u16) const {
    static char empty[1] = {0};
    return empty;
}

void* JUTResReference::getResource(JSUInputStream*, u32, JKRArchive*) {
    return nullptr;
}

void* JUTResReference::getResource(void const*, u32, JKRArchive*) {
    return nullptr;
}

void* JUTResReference::getResource(u32, JKRArchive*) {
    return nullptr;
}

void JUTPalette::storeTLUT(GXTlut, ResTLUT*) {}

void JUTPalette::storeTLUT(GXTlut, GXTlutFmt, JUTTransparency, u16, void*) {}

void J3DMaterialAnm::initialize() {}
void J3DMaterialAnm::setMatColorAnm(int, J3DMatColorAnm*) {}
void J3DMaterialAnm::setTexNoAnm(int, J3DTexNoAnm*) {}
void J3DMaterialAnm::calc(J3DMaterial*) const {}

J3DModelData* J3DModelLoaderDataBase::load(void const* i_data, u32) {
    return reinterpret_cast<J3DModelData*>(const_cast<void*>(i_data));
}

J3DModelData* J3DModelLoaderDataBase::loadBinaryDisplayList(const void* i_data, u32) {
    return reinterpret_cast<J3DModelData*>(const_cast<void*>(i_data));
}

s32 J3DModelData::newSharedDisplayList(u32) { return 0; }
void J3DModelData::makeSharedDL() {}
void J3DModelData::simpleCalcMaterial(u16, Mtx) {}
void J3DModelData::syncJ3DSysPointers() const {}
void J3DModelData::syncJ3DSysFlags() const {}

s32 J3DModel::setDeformData(J3DDeformData*, u32) { return 0; }
void J3DModel::calcWeightEnvelopeMtx() {}

void J3DShape::addTexMtxIndexInDL(_GXAttr, u32) {}
void J3DShape::addTexMtxIndexInVcd(_GXAttr) {}

void JMAVECScaleAdd(const Vec* vec1, const Vec* vec2, Vec* dst, f32 scale) {
    if (dst == nullptr) {
        return;
    }

    if (vec1 == nullptr || vec2 == nullptr) {
        *dst = {0.0f, 0.0f, 0.0f};
        return;
    }

    dst->x = vec1->x + vec2->x * scale;
    dst->y = vec1->y + vec2->y * scale;
    dst->z = vec1->z + vec2->z * scale;
}

J3DAnmBase* J3DAnmLoaderDataBase::load(void const* data, J3DAnmLoaderDataBaseFlag) {
    return reinterpret_cast<J3DAnmBase*>(const_cast<void*>(data));
}

void J3DAnmLoaderDataBase::setResource(J3DAnmBase*, void const*) {}

void* J3DClusterLoaderDataBase::load(void const* data) {
    return const_cast<void*>(data);
}

JPAResourceManager::JPAResourceManager(void const*, JKRHeap* pHeap)
    : mpHeap(pHeap), pResAry(nullptr), pTexAry(nullptr), resMaxNum(0), resRegNum(0), texMaxNum(0),
      texRegNum(0) {}

JPAResource* JPAResourceManager::getResource(u16 usrIdx) const {
    for (u16 i = 0; i < resRegNum; i++) {
        if (pResAry[i] != nullptr && pResAry[i]->getUsrIdx() == usrIdx) {
            return pResAry[i];
        }
    }
    return nullptr;
}

bool JPAResourceManager::checkUserIndexDuplication(u16 usrIdx) const {
    return getResource(usrIdx) != nullptr;
}

const ResTIMG* JPAResourceManager::swapTexture(ResTIMG const* img, char const*) {
    return img;
}

void JPAResourceManager::registRes(JPAResource* res) {
    if (pResAry != nullptr && resRegNum < resMaxNum) {
        pResAry[resRegNum++] = res;
    }
}

void JPAResourceManager::registTex(JPATexture* tex) {
    if (pTexAry != nullptr && texRegNum < texMaxNum) {
        pTexAry[texRegNum++] = tex;
    }
}

u32 JPAResourceManager::getResUserWork(u16 usrIdx) const {
    JPAResource* res = getResource(usrIdx);
    return res != nullptr ? res->getDyn()->getResUserWork() : 0;
}

JPAEmitterManager::JPAEmitterManager(u32 i_ptclNum, u32 i_emtrNum, JKRHeap*, u8 i_gidMax, u8 i_ridMax)
    : pEmtrUseList(nullptr), pResMgrAry(nullptr), pWd(nullptr), emtrNum(i_emtrNum), ptclNum(i_ptclNum),
      gidMax(i_gidMax), ridMax(i_ridMax) {}

JPABaseEmitter* JPAEmitterManager::createSimpleEmitterID(JGeometry::TVec3<f32> const&, u16, u8, u8,
                                                         JPAEmitterCallBack*, JPAParticleCallBack*) {
    return nullptr;
}

void JPAEmitterManager::calc(u8) {}
void JPAEmitterManager::draw(JPADrawInfo const*, u8) {}
void JPAEmitterManager::forceDeleteAllEmitter() {}
void JPAEmitterManager::forceDeleteGroupEmitter(u8) {}
void JPAEmitterManager::forceDeleteEmitter(JPABaseEmitter*) {}

void JPAEmitterManager::entryResourceManager(JPAResourceManager* resMgr, u8 resMgrID) {
    if (pResMgrAry != nullptr && resMgrID < ridMax) {
        pResMgrAry[resMgrID] = resMgr;
    }
}

void JPAEmitterManager::clearResourceManager(u8 resMgrID) {
    if (pResMgrAry != nullptr && resMgrID < ridMax) {
        pResMgrAry[resMgrID] = nullptr;
    }
}

void JPAEmitterManager::calcYBBCam() {}

void JPALoadExTex(JPAEmitterWorkData*) {}

void cBgW::GetTrans(cXyz* ptrans) const {
    if (ptrans != nullptr) {
        *ptrans = cXyz::Zero;
    }
}

void* dBgWKCol::initKCollision(void* data) {
    return data;
}
