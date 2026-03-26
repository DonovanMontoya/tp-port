/**
 * port/src/game_stubs.cpp
 * No-op stub definitions for game subsystem symbols required at link time
 * but whose full source files are not yet enabled in the port build.
 *
 * Add stubs here as new .cpp files are enabled and their transitive
 * dependencies surface at link time.
 */

#include "port/types.h"
#include "f_op/f_op_actor.h"
#include "d/d_com_inf_actor.h"
#include "d/d_kankyo.h"
#include "d/d_meter2.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "JSystem/J3DGraphAnimator/J3DCluster.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAttach.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "JSystem/J3DGraphBase/J3DPacket.h"
#include "JSystem/J3DGraphBase/J3DShape.h"
#include "JSystem/J3DGraphBase/J3DStruct.h"
#include "JSystem/J3DGraphBase/J3DSys.h"
#include "JSystem/J3DGraphBase/J3DVertex.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTNameTab.h"
#include "JSystem/JUtility/JUTResFont.h"
#include "m_Do/m_Do_mtx.h"
#include "Z2AudioLib/Z2Creature.h"
#include "dolphin/gd/GDBase.h"
#include "dolphin/gf/GFPixel.h"
#include "../../include/m_Do/m_Do_ext.h"

// ---------------------------------------------------------------------------
// Environment / Kankyo globals
// ---------------------------------------------------------------------------
dScnKy_env_light_c g_env_light;

// ---------------------------------------------------------------------------
// Actor info globals (d_com_inf_actor.h)
// ---------------------------------------------------------------------------
dComIfGoat_info_c g_dComIfGoat_gameInfo;
dComIfAc_info_c   g_dComIfAc_gameInfo;

// ---------------------------------------------------------------------------
// Message object HIO (d_msg_object.h)
// ---------------------------------------------------------------------------
dMsgObject_HIO_c g_MsgObject_HIO_c;

// ---------------------------------------------------------------------------
// Meter2 globals and stubs (d_meter2_info.h / d_meter2.h)
// ---------------------------------------------------------------------------

// Constructor/destructor stubs — no-op on PC
dMeter2Info_c::dMeter2Info_c()  {}
dMeter2Info_c::~dMeter2Info_c() {}

dMeter2Info_c g_meter2_info;

// dMeter2_c::emphasisButtonDelete — called from f_op_msg_mng.cpp when
// heap lock flag is 8; safe to no-op on PC.
int dMeter2_c::emphasisButtonDelete() { return 1; }

// dMeter2Info_c::getString — message string getter; no-op on PC.
void dMeter2Info_c::getString(u32 /*msgId*/, char* /*buf*/,
                               JMSMesgEntry_c* /*entry*/) {}
void dMeter2Info_c::getStringKana(u32 /*msgId*/, char* /*buf*/,
                                   JMSMesgEntry_c* /*entry*/) {}
void dMeter2Info_c::getStringKanji(u32 /*msgId*/, char* /*buf*/,
                                    JMSMesgEntry_c* /*entry*/) {}

// ---------------------------------------------------------------------------
// Event system stubs (f_op_actor.h: dEvt_info_c)
// ---------------------------------------------------------------------------
dEvt_info_c::dEvt_info_c()  {}
void dEvt_info_c::beforeProc() {}

// (fopAcM_cullingCheck, fopAcM_delete, fopAcM_DeleteHeap are now defined
//  in src/f_op/f_op_actor_mng.cpp — removed from stubs)

// ---------------------------------------------------------------------------
// Suspension system stub (d/actor/d_a_suspend.h: daSus_c)
// ---------------------------------------------------------------------------
#include "d/actor/d_a_suspend.h"
void daSus_c::check(fopAc_ac_c* /*actor*/) {}

// ---------------------------------------------------------------------------
// JUTGamePad rumble stubs (JSystem/JUtility/JUTGamePad.h)
// These are in libs/JSystem/src/JUtility/JUTGamePad.cpp which is not yet
// linked — provide minimal stubs so the port links cleanly.
// ---------------------------------------------------------------------------
#include "JSystem/JUtility/JUTGamePad.h"

void JUTGamePad::CRumble::stopPatternedRumble(s16 /*port*/)          {}
void JUTGamePad::CRumble::stopPatternedRumbleAtThePeriod()            {}

// ---------------------------------------------------------------------------
// J3DSys global and statics (J3DGraphBase/J3DSys.h)
// ---------------------------------------------------------------------------
J3DSys j3dSys;
Mtx  J3DSys::mCurrentMtx;
Vec  J3DSys::mCurrentS = {};
Vec  J3DSys::mParentS = {1.0f, 1.0f, 1.0f};
J3DTexCoordScaleInfo J3DSys::sTexCoordScaleTable[8] = {};
u32 j3dDefaultViewNo = 0;

// ---------------------------------------------------------------------------
// Background system global — all methods are inline stubs in port/include/d/d_bg_s.h
// ---------------------------------------------------------------------------
#include "d/d_bg_s.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_bg_s_spl_grp_chk.h"
#include "d/d_bg_s_wtr_chk.h"
#include "d/d_path.h"

// Background system accessor — returns a static stub dBgS
static dBgS s_bgsp;
dBgS& dComIfG_Bgsp() { return s_bgsp; }

// Path helper
u8 dPath_GetPolyRoomPathVec(cBgS_PolyInfo const&, cXyz*, int*) { return 0; }

// daTagStream_c static member
#include "d/actor/d_a_tag_stream.h"
daTagStream_c* daTagStream_c::m_top = nullptr;
int daTagStream_c::checkArea(cXyz const*) { return 0; }

// dEnemyItem_c static member
#include "d/d_item.h"
u8* dEnemyItem_c::mData = nullptr;

// ---------------------------------------------------------------------------
// g_blackColor — fade color global used by d_s_logo.cpp
// ---------------------------------------------------------------------------
#include "dolphin/gx/GXStruct.h"
GXColor g_blackColor = { 0, 0, 0, 255 };

// ---------------------------------------------------------------------------
// JFWDisplay static
// ---------------------------------------------------------------------------
#include "JSystem/JFramework/JFWDisplay.h"
JFWDisplay* JFWDisplay::sManager = nullptr;

// ---------------------------------------------------------------------------
// mDoGph_gInf_c statics
// ---------------------------------------------------------------------------
#include "m_Do/m_Do_graphic.h"
JUTFader*   mDoGph_gInf_c::mFader = nullptr;

// ---------------------------------------------------------------------------
// dTres_c and dMpath_c createWork stubs
// ---------------------------------------------------------------------------
#include "d/d_tresure.h"
int dTres_c::createWork() { return 0; }

#include "d/d_map_path_dmap.h"
void dMpath_c::createWork() {}

// ---------------------------------------------------------------------------
// dComIfGs_onActor — declared non-inline in f_op_actor_mng.h; stub here
// ---------------------------------------------------------------------------
void dComIfGs_onActor(int /*bitNo*/, int /*roomNo*/) {}

// ---------------------------------------------------------------------------
// mDoLib_clipper statics (m_Do/m_Do_lib.h)
// ---------------------------------------------------------------------------
#include "m_Do/m_Do_lib.h"
J3DUClipper mDoLib_clipper::mClipper;
f32         mDoLib_clipper::mSystemFar  = 100000.0f;
f32         mDoLib_clipper::mFovyRate   = 1.0f;

// ---------------------------------------------------------------------------
// dScnPly_c statics — defined in d_s_play.cpp which is not yet enabled
// ---------------------------------------------------------------------------
#include "d/d_s_play.h"
s8 dScnPly_c::pauseTimer     = 0;
s8 dScnPly_c::nextPauseTimer = 0;

// ---------------------------------------------------------------------------
// Low-level GD / GF / JMath shims used by m_Do_ext.cpp
// ---------------------------------------------------------------------------
extern "C" GDLObj* __GDCurrentDL = nullptr;

extern "C" void GDInitGDLObj(GDLObj* dl, void* start, u32 length) {
    if (dl == nullptr) {
        return;
    }

    dl->start = static_cast<u8*>(start);
    dl->length = length;
    dl->ptr = dl->start;
    dl->top = dl->start != nullptr ? dl->start + length : nullptr;
}

void GFSetBlendModeEtc(GXBlendMode, GXBlendFactor, GXBlendFactor, GXLogicOp, u8, u8, u8) {}
void GFSetZMode(u8, GXCompare, u8) {}

void JMAEulerToQuat(s16, s16, s16, Quaternion* quat) {
    if (quat == nullptr) {
        return;
    }

    quat->x = 0.0f;
    quat->y = 0.0f;
    quat->z = 0.0f;
    quat->w = 1.0f;
}

void JMAQuatLerp(const Quaternion* p, const Quaternion* q, f32 t, Quaternion* out) {
    if (out == nullptr) {
        return;
    }

    if (p == nullptr && q == nullptr) {
        out->x = 0.0f;
        out->y = 0.0f;
        out->z = 0.0f;
        out->w = 1.0f;
        return;
    }

    if (p == nullptr) {
        *out = *q;
        return;
    }

    if (q == nullptr) {
        *out = *p;
        return;
    }

    out->x = p->x + (q->x - p->x) * t;
    out->y = p->y + (q->y - p->y) * t;
    out->z = p->z + (q->z - p->z) * t;
    out->w = p->w + (q->w - p->w) * t;
}

// ---------------------------------------------------------------------------
// J3D statics and no-op methods surfaced by m_Do_ext.cpp
// ---------------------------------------------------------------------------
J3DMtxBuffer* J3DMtxCalc::mMtxBuffer = nullptr;
J3DJoint* J3DMtxCalc::mJoint = nullptr;
J3DMtxCalc* J3DJoint::mCurrentMtxCalc = nullptr;
GDLObj J3DDisplayListObj::sGDLObj = {};
s32 J3DDisplayListObj::sInterruptFlag = 0;
void* J3DShape::sOldVcdVatCmd = nullptr;
bool J3DShape::sEnvelopeFlag = false;

void J3DFrameCtrl::init(s16 endFrame) {
    mAttribute = EMode_NONE;
    mState = 0;
    mStart = 0;
    mEnd = endFrame;
    mLoop = 0;
    mRate = 1.0f;
    mFrame = 0.0f;
}

BOOL J3DFrameCtrl::checkPass(f32 frame) {
    return mRate >= 0.0f ? (mFrame >= frame) : (mFrame <= frame);
}

void J3DFrameCtrl::update() {
    mFrame += mRate;
}

void J3DDeformData::setAnm(J3DAnmCluster*) {}

int J3DDrawBuffer::entryImm(J3DPacket* pPacket, u16) {
    mpCallBackPacket = pPacket;
    return 0;
}

void J3DTexMtxInfo::setEffectMtx(Mtx mtx) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 4; ++c) {
            mEffectMtx[r][c] = mtx[r][c];
        }
    }
    mEffectMtx[3][0] = 0.0f;
    mEffectMtx[3][1] = 0.0f;
    mEffectMtx[3][2] = 0.0f;
    mEffectMtx[3][3] = 1.0f;
}

void J3DMaterialAnm::setTexMtxAnm(int idx, J3DTexMtxAnm* anm) {
    if (idx < 0 || idx >= 8) {
        return;
    }
    mTexMtxAnm[idx] = anm != nullptr ? *anm : J3DTexMtxAnm();
}

void J3DMaterialAnm::setTevColorAnm(int idx, J3DTevColorAnm* anm) {
    if (idx < 0 || idx >= 4) {
        return;
    }
    mTevColorAnm[idx] = anm != nullptr ? *anm : J3DTevColorAnm();
}

void J3DMaterialAnm::setTevKColorAnm(int idx, J3DTevKColorAnm* anm) {
    if (idx < 0 || idx >= 4) {
        return;
    }
    mTevKColorAnm[idx] = anm != nullptr ? *anm : J3DTevKColorAnm();
}

void J3DShapePacket::drawFast() {
    draw();
}

void J3DAnmColor::searchUpdateMaterialID(J3DMaterialTable*) {}

void J3DAnmTevRegKey::searchUpdateMaterialID(J3DMaterialTable*) {}

void J3DAnmTexPattern::searchUpdateMaterialID(J3DMaterialTable*) {}

void J3DAnmTextureSRTKey::searchUpdateMaterialID(J3DMaterialTable*) {}

int J3DMaterialTable::entryTexNoAnimator(J3DAnmTexPattern*) {
    return 0;
}

int J3DMaterialTable::entryTevRegAnimator(J3DAnmTevRegKey*) {
    return 0;
}

int J3DMaterialTable::entryTexMtxAnimator(J3DAnmTextureSRTKey*) {
    return 0;
}

int J3DMaterialTable::entryMatColorAnimator(J3DAnmColor*) {
    return 0;
}

void J3DVertexBuffer::init() {
    mVtxData = nullptr;
    for (int i = 0; i < 2; ++i) {
        mVtxPosArray[i] = nullptr;
        mVtxNrmArray[i] = nullptr;
        mVtxColArray[i] = nullptr;
        mTransformedVtxPosArray[i] = nullptr;
        mTransformedVtxNrmArray[i] = nullptr;
    }
    mCurrentVtxPos = nullptr;
    mCurrentVtxNrm = nullptr;
    mCurrentVtxCol = nullptr;
}

J3DVertexBuffer::~J3DVertexBuffer() {}

void J3DModel::initialize() {
    mModelData = nullptr;
    mFlags = 0;
    mDiffFlag = 0;
    mCalcCallBack = nullptr;
    mUserArea = 0;
    mBaseScale.x = 1.0f;
    mBaseScale.y = 1.0f;
    mBaseScale.z = 1.0f;
    MTXIdentity(mBaseTransformMtx);
    MTXIdentity(mInternalView);
    mMtxBuffer = nullptr;
    mMatPacket = nullptr;
    mShapePacket = nullptr;
    mDeformData = nullptr;
    mSkinDeform = nullptr;
    mVtxColorCalc = nullptr;
    mUnkCalc1 = nullptr;
    mUnkCalc2 = nullptr;
}

s32 J3DModel::entryModelData(J3DModelData* modelData, u32 mdlFlags, u32) {
    mModelData = modelData;
    mFlags = mdlFlags;
    return 0;
}

s32 J3DModel::newDifferedDisplayList(u32) {
    return 0;
}

void J3DModel::lock() {}
void J3DModel::unlock() {}
void J3DModel::diff() {}
void J3DModel::update() {}
void J3DModel::entry() {}
void J3DModel::calc() {}
void J3DModel::calcMaterial() {}
void J3DModel::calcDiffTexMtx() {}
void J3DModel::viewCalc() {}

int J3DPacket::entry(J3DDrawBuffer*) {
    return 0;
}

void J3DPacket::draw() {}

const char* JUTNameTab::getName(u16 index) const {
    if (mNameTable == nullptr || index >= mNameNum) {
        return "";
    }
    return mNameTable->getName(index);
}

void J3DDisplayListObj::callDL() const {}
void J3DShape::loadPreDrawSetting() const {}

void J3DMtxCalcJ3DSysInitMaya::init(const Vec& scale, const Mtx&) {
    j3dSys.mCurrentS = scale;
}

void J3DMtxCalcCalcTransformMaya::calcTransform(const J3DTransformInfo&) {}

// ---------------------------------------------------------------------------
// JUTResFont host-side shim
// ---------------------------------------------------------------------------
JUTResFont::JUTResFont(const ResFONT* font, JKRHeap*) : JUTFont() {
    initialize_state();
    mResFont = font;
    mInf1Ptr = font != nullptr ? const_cast<ResFONT::INF1*>(reinterpret_cast<const ResFONT::INF1*>(font->data)) : nullptr;
}

JUTResFont::~JUTResFont() {}

void JUTResFont::initialize_state() {
    mWidth = 0;
    mHeight = 0;
    mTexPageIdx = 0;
    mResFont = nullptr;
    mInf1Ptr = nullptr;
    mMemBlocks = nullptr;
    mpWidthBlocks = nullptr;
    mpGlyphBlocks = nullptr;
    mpMapBlocks = nullptr;
    mWid1BlockNum = 0;
    mGly1BlockNum = 0;
    mMap1BlockNum = 0;
    field_0x66 = 0;
    mMaxCode = 0;
    mIsLeadByte = nullptr;
}

void JUTResFont::setGX() {}
void JUTResFont::setGX(JUtility::TColor, JUtility::TColor) {}
f32 JUTResFont::drawChar_scale(f32, f32, f32, f32, int, bool) { return 0.0f; }
int JUTResFont::getLeading() const { return mInf1Ptr != nullptr ? mInf1Ptr->leading : 0; }
s32 JUTResFont::getAscent() const { return mInf1Ptr != nullptr ? mInf1Ptr->ascent : 0; }
s32 JUTResFont::getDescent() const { return mInf1Ptr != nullptr ? mInf1Ptr->descent : 0; }
s32 JUTResFont::getHeight() const { return mHeight; }
s32 JUTResFont::getWidth() const { return mWidth; }
void JUTResFont::getWidthEntry(int, JUTFont::TWidth* width) const {
    if (width == nullptr) {
        return;
    }
    width->field_0x0 = 0;
    width->field_0x1 = static_cast<u8>(mWidth);
}
s32 JUTResFont::getCellWidth() const { return mWidth; }
s32 JUTResFont::getCellHeight() const { return mHeight; }
int JUTResFont::getFontType() const { return mInf1Ptr != nullptr ? mInf1Ptr->fontType : 0; }
ResFONT* JUTResFont::getResFont() const { return const_cast<ResFONT*>(mResFont); }
bool JUTResFont::isLeadByte(int) const { return false; }
void JUTResFont::loadImage(int, GXTexMapID) {}
void JUTResFont::setBlock() {}

// ---------------------------------------------------------------------------
// Z2 audio shims used by morph / zel anime paths
// ---------------------------------------------------------------------------
Z2SoundHandles::Z2SoundHandles() : handleNum_(0) {}
Z2SoundHandles::~Z2SoundHandles() {}

Z2SoundObjBase::Z2SoundObjBase()
    : soundStarter_(nullptr), pos_(nullptr), field_0x1c(0), reverb_(0), alive_(false) {}

Z2SoundObjBase::~Z2SoundObjBase() {}

void Z2SoundObjBase::framework(u32, s8) {}
void Z2SoundObjBase::dispose() {}
bool Z2SoundObjBase::stopOK(Z2SoundHandlePool&) { return true; }
void Z2SoundObjBase::init(Vec* posPtr, u8) {
    pos_ = reinterpret_cast<JGeometry::TVec3<f32>*>(posPtr);
    alive_ = true;
}
Z2SoundHandlePool* Z2SoundObjBase::startSound(JAISoundID, u32, s8) { return nullptr; }
Z2SoundHandlePool* Z2SoundObjBase::startLevelSound(JAISoundID, u32, s8) { return nullptr; }

Z2SoundObjAnime::Z2SoundObjAnime()
    : animation_(nullptr), field_0x24(0), curSoundFrame_(0.0f), startSoundFrame_(0.0f),
      endSoundFrame_(0.0f), startSoundIndex_(0), endSoundIndex_(0), field_0x3c(0),
      curSoundIndex_(0), reverse_(false) {}

void Z2SoundObjAnime::init(Vec* posPtr, u8 handleNum) {
    Z2SoundObjBase::init(posPtr, handleNum);
}

void Z2SoundObjAnime::initAnime(void* animation, bool, f32 startFrame, f32) {
    animation_ = static_cast<JAUSoundAnimation*>(animation);
    curSoundFrame_ = startFrame;
}

void Z2SoundObjAnime::updateAnime(f32 frame, f32) {
    curSoundFrame_ = frame;
}

void Z2Creature::initAnime(void* animation, bool loop, f32 startFrame, f32 endFrame) {
    mSoundObjAnime.initAnime(animation, loop, startFrame, endFrame);
}

void Z2Creature::updateAnime(f32 frame, f32 rate) {
    mSoundObjAnime.updateAnime(frame, rate);
}

// ---------------------------------------------------------------------------
// Missing globals
// ---------------------------------------------------------------------------
u8 g_printOtherHeapDebug = 0;

// ---------------------------------------------------------------------------
// Out-of-line packet destructors declared in m_Do_ext.h
// ---------------------------------------------------------------------------
mDoExt_offCupOnAupPacket::~mDoExt_offCupOnAupPacket() {}
mDoExt_onCupOffAupPacket::~mDoExt_onCupOffAupPacket() {}
