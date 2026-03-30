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
#include "d/d_attention.h"
#include "d/d_com_inf_actor.h"
#include "d/d_eye_hl.h"
#include "d/d_kankyo.h"
#include "d/d_meter2.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "d/d_save_HIO.h"
#include "d/d_s_play.h"
#include "d/d_stage.h"
#include "d/d_vibration.h"
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
#include "JSystem/J2DGraph/J2DPicture.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "m_Do/m_Do_mtx.h"
#include "Z2AudioLib/Z2Creature.h"
#include "dolphin/gd/GDBase.h"
#include "dolphin/gf/GFPixel.h"
#include "../../include/m_Do/m_Do_ext.h"
#include "../../include/d/d_menu_collect.h"
#include "../../include/d/d_pane_class_alpha.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

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
dSvBit_HIO_c g_save_bit_HIO;

dSvBit_childTransformHIO_c::dSvBit_childTransformHIO_c() {}
void dSvBit_childTransformHIO_c::init() {}
dSvBit_childDarknessHIO_c::dSvBit_childDarknessHIO_c() {}
void dSvBit_childDarknessHIO_c::init() {}
dSvBit_childOtherHIO_c::dSvBit_childOtherHIO_c() {}
void dSvBit_childOtherHIO_c::init() {}
dSvBit_childTbPerfectionHIO_c::dSvBit_childTbPerfectionHIO_c() {}
void dSvBit_childTbPerfectionHIO_c::init() {}
dSvBit_childSwZoneHIO_c::dSvBit_childSwZoneHIO_c() {}
void dSvBit_childSwZoneHIO_c::init() {}
dSvBit_childSwPerfectionHIO_c::dSvBit_childSwPerfectionHIO_c() {}
void dSvBit_childSwPerfectionHIO_c::init() {}
dSvBit_childSwOneZoneHIO_c::dSvBit_childSwOneZoneHIO_c() {}
void dSvBit_childSwOneZoneHIO_c::init() {}
dSvBit_childSwDungeonHIO_c::dSvBit_childSwDungeonHIO_c() {}
void dSvBit_childSwDungeonHIO_c::init() {}
dSvBit_childItZoneHIO_c::dSvBit_childItZoneHIO_c() {}
void dSvBit_childItZoneHIO_c::init() {}
dSvBit_childItPerfectionHIO_c::dSvBit_childItPerfectionHIO_c() {}
void dSvBit_childItPerfectionHIO_c::init() {}
dSvBit_childItOneZoneHIO_c::dSvBit_childItOneZoneHIO_c() {}
void dSvBit_childItOneZoneHIO_c::init() {}
dSvBit_childItDungeonHIO_c::dSvBit_childItDungeonHIO_c() {}
void dSvBit_childItDungeonHIO_c::init() {}

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
void dMeter2Info_setCloth(u8 /*i_clothId*/, bool /*i_offItemBit*/) {}
void dMeter2Info_setSword(u8 /*i_itemId*/, bool /*i_offItemBit*/) {}
void dMeter2Info_setShield(u8 /*i_itemId*/, bool /*i_offItemBit*/) {}

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
void daSus_c::reset() {}
void daSus_c::execute() {}
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

// Path helper
u8 dPath_GetPolyRoomPathVec(cBgS_PolyInfo const&, cXyz*, int*) { return 0; }

// Stage helpers used by the real opening/gameplay scene bootstrap
bool dStage_roomControl_c::resetArchiveBank(int) { return true; }
u32 dStage_stagInfo_GetParticleNo(stage_stag_info_class*, int) { return 255; }
void dStage_infoCreate() {}
void dStage_Create() {}
void dStage_Delete() {}

// daTagStream_c static member
#include "d/actor/d_a_tag_stream.h"
daTagStream_c* daTagStream_c::m_top = nullptr;
int daTagStream_c::checkArea(cXyz const*) { return 0; }

// daYkgr_c process-wide state
#include "d/actor/d_a_ykgr.h"
JPABaseEmitter* daYkgr_c::m_emitter = nullptr;
bool daYkgr_c::m_flag = false;
bool daYkgr_c::m_alpha_flag = true;
u8 daYkgr_c::m_alpha = 255;
f32 daYkgr_c::m_aim_rate = 0.0f;
dPath* daYkgr_c::m_path = nullptr;

// dEnemyItem_c static member
#include "d/d_item.h"
u8* dEnemyItem_c::mData = nullptr;

// ---------------------------------------------------------------------------
// JFWDisplay static
// ---------------------------------------------------------------------------
#include "JSystem/JFramework/JFWDisplay.h"
JFWDisplay* JFWDisplay::sManager = nullptr;

// ---------------------------------------------------------------------------
// mDoGph_gInf_c statics
// ---------------------------------------------------------------------------
#include "m_Do/m_Do_graphic.h"

// ---------------------------------------------------------------------------
// dTres_c and dMpath_c createWork stubs
// ---------------------------------------------------------------------------
#include "d/d_tresure.h"
int dTres_c::createWork() { return 0; }
void dTres_c::create() {}
void dTres_c::remove() {}

#include "d/d_map_path_dmap.h"
void dMpath_c::createWork() {}
void dMpath_c::create() {}
void dMpath_c::remove() {}

// Opening/gameplay scene service stubs
dAttention_c::dAttention_c(fopAc_ac_c* actor, u32 padNo) { Init(actor, padNo); }
int dAttention_c::Run() { return 1; }
void dAttention_c::Draw() {}

void dScnPly_env_otherHIO_c::listenPropertyEvent(const JORPropertyEvent*) {}
void dScnPly_env_debugHIO_c::listenPropertyEvent(const JORPropertyEvent*) {}

dEyeHL_c* dEyeHL_mng_c::m_obj = nullptr;
void dEyeHL_mng_c::update() {}

int dVibration_c::Run() { return 1; }
void dVibration_c::Init() {}
void dVibration_c::Pause() {}
void dVibration_c::Remove() {}

f32 dMenu_Collect3D_c::mViewOffsetY = -100.0f;

CPaneMgrAlpha::CPaneMgrAlpha()
    : mPane(new J2DPane()), heap(nullptr), mpFirstStackAlpha(nullptr), field_0x10(nullptr),
      mChildPaneCount(0), mAlphaTimer(0), mInitAlpha(255), mFlags(0) {
    if (mPane != nullptr) {
        mPane->setAlpha(mInitAlpha);
    }
}

CPaneMgrAlpha::CPaneMgrAlpha(J2DScreen* screen, u64 tag, u8, JKRExpHeap* expHeap)
    : CPaneMgrAlpha() {
    heap = expHeap;
    if (screen != nullptr) {
        J2DPane* pane = screen->search(tag);
        if (pane != nullptr) {
            delete mPane;
            mPane = pane;
            mInitAlpha = pane->getAlpha();
        }
    }
}

CPaneMgrAlpha::~CPaneMgrAlpha() {}

void CPaneMgrAlpha::setAlpha(u8 alpha) {
    if (mPane != nullptr) {
        mPane->setAlpha(alpha);
    }
}

bool CPaneMgrAlpha::alphaAnime(s16 timer, u8, u8 endAlpha, u8) {
    if (mPane != nullptr) {
        mPane->setAlpha(endAlpha);
    }
    mAlphaTimer = timer;
    return true;
}

bool CPaneMgrAlpha::alphaAnimeLoop(s16 timer, u8, u8 endAlpha, u8) {
    if (mPane != nullptr) {
        mPane->setAlpha(endAlpha);
    }
    mAlphaTimer = timer;
    return true;
}

namespace {
J2DPane* tp_stub_title_root_pane() {
    static J2DPane pane;
    return &pane;
}

J2DTextBox* tp_stub_title_textbox() {
    static J2DTextBox box;
    return &box;
}
}  // namespace

JGeometry::TBox2<f32> J2DPane::static_mBounds;

J2DPane::J2DPane()
    : field_0x4(0), mKind(0), mInfoTag(0), mUserInfoTag(0), mBounds(), mGlobalBounds(),
      mClipRect(), mVisible(true), mCullMode(0), mAlpha(255), mColorAlpha(255),
      mIsInfluencedAlpha(false), mConnected(false), mRotAxis('z'), mBasePosition(0),
      mRotateX(0.0f), mRotateY(0.0f), mRotateZ(0.0f), mRotateOffsetX(0.0f),
      mRotateOffsetY(0.0f), mScaleX(1.0f), mScaleY(1.0f), mTranslateX(0.0f),
      mTranslateY(0.0f), mPaneTree(this), mTransform(nullptr)
#if !(PLATFORM_WII || PLATFORM_SHIELD)
      , _fc(0)
#endif
{
    std::memset(mPositionMtx, 0, sizeof(mPositionMtx));
    std::memset(mGlobalMtx, 0, sizeof(mGlobalMtx));
}

J2DPane::~J2DPane() {}

void J2DPane::move(f32 x, f32 y) {
    mTranslateX = x;
    mTranslateY = y;
}

void J2DPane::add(f32 x, f32 y) {
    mTranslateX += x;
    mTranslateY += y;
}

void J2DPane::resize(f32 x, f32 y) {
    mBounds.f.x = mBounds.i.x + x;
    mBounds.f.y = mBounds.i.y + y;
}

J2DPane* J2DPane::search(u64) { return nullptr; }
J2DPane* J2DPane::searchUserInfo(u64) { return nullptr; }
bool J2DPane::isUsed(const ResTIMG*) { return false; }
bool J2DPane::isUsed(const ResFONT*) { return false; }
void J2DPane::setCullBack(_GXCullMode cmode) { mCullMode = static_cast<u8>(cmode); }
void J2DPane::makeMatrix(f32, f32, f32, f32) {}
void J2DPane::clearAnmTransform() {}
void J2DPane::setAnimation(J2DAnmBase*) {}
void J2DPane::setAnimation(J2DAnmTransform*) {}
void J2DPane::setVisibileAnimation(J2DAnmVisibilityFull*) {}
void J2DPane::setVtxColorAnimation(J2DAnmVtxColor*) {}
const J2DAnmTransform* J2DPane::animationTransform(const J2DAnmTransform* transform) {
    return transform;
}
const J2DAnmTransform* J2DPane::animationPane(const J2DAnmTransform* transform) {
    return transform;
}

J2DScreen::J2DScreen() : J2DPane(), mScissor(false), mMaterialNum(0), mMaterials(nullptr),
                         mTexRes(nullptr), mFontRes(nullptr), mNameTable(nullptr), mColor() {}

J2DScreen::~J2DScreen() {}

u16 J2DScreen::getTypeID() const { return 17; }
void J2DScreen::calcMtx() {}
void J2DScreen::drawSelf(f32, f32, Mtx*) {}
J2DPane* J2DScreen::searchUserInfo(u64) { return nullptr; }
bool J2DScreen::isUsed(ResTIMG const*) { return false; }
bool J2DScreen::isUsed(ResFONT const*) { return false; }
void J2DScreen::setAnimation(J2DAnmColor*) {}
void J2DScreen::setAnimation(J2DAnmTextureSRTKey*) {}
void J2DScreen::setAnimation(J2DAnmVtxColor*) {}
void J2DScreen::setAnimation(J2DAnmTexPattern*) {}
void J2DScreen::setAnimation(J2DAnmVisibilityFull*) {}
void J2DScreen::setAnimation(J2DAnmTevRegKey*) {}
void J2DScreen::setAnimation(J2DAnmBase*) {}
void J2DScreen::setAnimationVF(J2DAnmVisibilityFull*) {}
void J2DScreen::setAnimationVC(J2DAnmVtxColor*) {}
J2DPane* J2DScreen::createPane(J2DScrnBlockHeader const&, JSURandomInputStream*, J2DPane*, u32) {
    return nullptr;
}
J2DPane* J2DScreen::createPane(J2DScrnBlockHeader const&, JSURandomInputStream*, J2DPane*, u32,
                               JKRArchive*) {
    return nullptr;
}

bool J2DScreen::setPriority(char const*, u32, JKRArchive*) {
    return true;
}

void J2DScreen::draw(f32, f32, J2DGrafContext const*) {}

J2DPane* J2DScreen::search(u64 tag) {
    if (tag == MULTI_CHAR('n_all')) {
        return tp_stub_title_root_pane();
    }

    return tp_stub_title_textbox();
}

J2DTextBox::J2DTextBox()
    : J2DPane(), mFont(nullptr), mCharColor(), mGradientColor(), field_0x10c(0.0f),
      field_0x110(0.0f), mCharSpacing(0.0f), mLineSpacing(0.0f), mFontSizeX(0.0f),
      mFontSizeY(0.0f), mStringPtr(nullptr), mWhiteColor(), mBlackColor(), mFlags(0),
      mTextFontOwned(false), mStringLength(0) {}

J2DTextBox::~J2DTextBox() {
    ::free(mStringPtr);
    mStringPtr = nullptr;
}

u16 J2DTextBox::getTypeID() const { return 19; }
void J2DTextBox::resize(f32 x, f32 y) { J2DPane::resize(x, y); }
bool J2DTextBox::setConnectParent(bool connected) { mConnected = connected; return true; }
void J2DTextBox::drawSelf(f32, f32, Mtx*) {}
void J2DTextBox::drawSelf(f32, f32) {}
bool J2DTextBox::isUsed(ResTIMG const*) { return false; }
bool J2DTextBox::isUsed(ResFONT const*) { return false; }
void J2DTextBox::rewriteAlpha() {}
void J2DTextBox::draw(f32, f32) {}
void J2DTextBox::draw(f32, f32, f32, J2DTextBoxHBinding) {}
void J2DTextBox::setFont(JUTFont* font) { mFont = font; }
bool J2DTextBox::setBlack(JUtility::TColor black) {
    mBlackColor = black;
    return true;
}

char* J2DTextBox::getStringPtr() const {
    return mStringPtr;
}

s32 J2DTextBox::setString(s16 length, char const* string, ...) {
    if (length <= 0) {
        return 0;
    }

    if (mStringPtr == nullptr || mStringLength < static_cast<u16>(length)) {
        ::free(mStringPtr);
        mStringPtr = static_cast<char*>(::calloc(static_cast<size_t>(length), 1));
        mStringLength = length;
    }

    va_list args;
    va_start(args, string);
    std::vsnprintf(mStringPtr, static_cast<size_t>(length), string, args);
    va_end(args);
    return std::strlen(mStringPtr);
}

J2DPicture::J2DPicture() : J2DPane() {
    mTexture[0] = nullptr;
    mTexture[1] = nullptr;
    mTextureNum = 0;
    field_0x109 = 0;
}

J2DPicture::~J2DPicture() {}

void J2DPicture::drawSelf(f32, f32) {}
void J2DPicture::drawSelf(f32, f32, Mtx*) {}
void J2DPicture::initiate(ResTIMG const*, ResTLUT const*) {}
bool J2DPicture::prepareTexture(u8) { return true; }
bool J2DPicture::insert(ResTIMG const*, JUTPalette*, u8, f32) { return true; }
bool J2DPicture::insert(char const*, JUTPalette*, u8, f32) { return true; }
bool J2DPicture::insert(JUTTexture*, u8, f32) { return true; }
bool J2DPicture::remove(u8) { return true; }
bool J2DPicture::remove(JUTTexture*) { return true; }
void J2DPicture::draw(f32, f32, f32, f32, bool, bool, bool) {}
void J2DPicture::drawOut(JGeometry::TBox2<f32> const&, JGeometry::TBox2<f32> const&) {}
void J2DPicture::setBlendColorRatio(f32, f32) {}
void J2DPicture::setBlendAlphaRatio(f32, f32) {}
const ResTIMG* J2DPicture::changeTexture(ResTIMG const* timg, u8) { return timg; }
const ResTIMG* J2DPicture::changeTexture(char const*, u8) { return nullptr; }
const ResTIMG* J2DPicture::changeTexture(ResTIMG const* timg, u8, JUTPalette*) { return timg; }
const ResTIMG* J2DPicture::changeTexture(char const*, u8, JUTPalette*) { return nullptr; }
u8 J2DPicture::getUsableTlut(u8 value) { return value; }
bool J2DPicture::isUsed(ResTIMG const*) { return false; }
void J2DPicture::drawFullSet(f32, f32, f32, f32, Mtx*) {}
void J2DPicture::drawTexCoord(f32, f32, f32, f32, s16, s16, s16, s16, s16, s16, s16, s16,
                              Mtx*) {}

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
