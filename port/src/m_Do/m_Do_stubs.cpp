/**
 * port/src/m_Do/m_Do_stubs.cpp
 * PC port stubs for m_Do subsystem symbols needed by SComponent files.
 * The full m_Do_graphic / m_Do_controller_pad implementations have too many
 * GC-specific dependencies to compile on PC; this file supplies the
 * minimum set of symbols that c_API*.cpp pulls in at link time.
 */
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "JSystem/JFramework/JFWSystem.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTException.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "Z2AudioLib/Z2AudioMgr.h"
#include "Z2AudioLib/Z2SoundMgr.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "JSystem/JKernel/JKRAramStream.h"
#include "JSystem/JKernel/JKRDvdAramRipper.h"
#include "dolphin/pad.h"
#include "../window/window.h"

#include <cmath>

template <>
Z2AudioMgr* JASGlobalInstance<Z2AudioMgr>::sInstance = nullptr;

namespace {
#ifndef TP_PORT_USE_REAL_MDO_GRAPHIC
JUTFader gPortFader;
u8 gPortFrameBufferPixels[(FB_WIDTH / 2) * (FB_HEIGHT / 2) * 4] = {};
u8 gPortZBufferPixels[(FB_WIDTH / 2) * (FB_HEIGHT / 2) * 4] = {};
u8 gPortWideFrameBufferPixels[FB_WIDTH * FB_HEIGHT * 4] = {};
ResTIMG gPortFrameBufferTimg = {};
ResTIMG gPortZBufferTimg = {};
ResTIMG gPortWideFrameBufferTimg = {};
u32 gPortPresentedFrames = 0;
#endif
}  // namespace

void JUTGamePad::CButton::clear() {
    mButton = 0;
    mTrigger = 0;
    mRelease = 0;
    mAnalogA = 0;
    mAnalogB = 0;
    mAnalogL = 0;
    mAnalogR = 0;
    mAnalogLf = 0.0f;
    mAnalogRf = 0.0f;
    mRepeat = 0;
    mRepeatCount = 0;
    mRepeatStart = 0;
    mRepeatMask = 0;
    mRepeatDelay = 0;
    mRepeatRate = 0;
}

void JUTGamePad::CStick::clear() {
    mPosX = 0.0f;
    mPosY = 0.0f;
    mValue = 0.0f;
    mAngle = 0;
    mRawX = 0;
    mRawY = 0;
}

void JUTGamePad::CRumble::clear(JUTGamePad* /*pad*/) {
    mFrame = 0;
    mLength = 0;
    mPattern = nullptr;
    mFrameCount = 0;
    field_0x10 = nullptr;
}

JUTGamePad::JUTGamePad(EPadPort port) : mRumble(this), mLink(this) {
    mPortNum = port;
    mErrorStatus = 0;
    mPadRecord = nullptr;
    mPadReplay = nullptr;
    mButtonReset.mReset = false;
    mResetHoldStartTime = 0;
    field_0xa8 = 0;
    mButton.clear();
    mMainStick.clear();
    mSubStick.clear();
}

JUTGamePad::~JUTGamePad() {}

u32 JUTGamePad::read() {
    PADRead(mPadStatus);

    for (int i = 0; i < 4; ++i) {
        if (mPadStatus[i].err == PAD_ERR_NONE) {
            u32 stick_status = mPadMStick[i].update(mPadStatus[i].stickX, mPadStatus[i].stickY,
                                                    sStickMode, EMainStick,
                                                    mPadButton[i].mButton)
                               << 24;
            stick_status |= mPadSStick[i].update(mPadStatus[i].substickX, mPadStatus[i].substickY,
                                                 sStickMode, ESubStick,
                                                 mPadButton[i].mButton)
                            << 16;
            mPadButton[i].update(&mPadStatus[i], stick_status);
        } else {
            mPadMStick[i].update(0, 0, sStickMode, EMainStick, 0);
            mPadSStick[i].update(0, 0, sStickMode, ESubStick, 0);
            mPadButton[i].update(nullptr, 0);
        }
    }

    for (int i = 0; i < 4; ++i) {
        JUTGamePad* pad = mDoCPd_c::m_gamePad[i];
        if (pad != nullptr) {
            pad->update();
        }
    }

    return 0;
}

void JUTGamePad::update() {
    if (mPortNum < EPort1 || mPortNum > EPort4) {
        return;
    }

    mButton = mPadButton[mPortNum];
    mMainStick = mPadMStick[mPortNum];
    mSubStick = mPadSStick[mPortNum];
    mErrorStatus = mPadStatus[mPortNum].err;
}

void JUTGamePad::clearForReset() {
    C3ButtonReset::sResetOccurred = false;
    C3ButtonReset::sResetSwitchPushing = false;
}

JUTGamePad* JUTGamePad::getGamePad(int port) {
    if (port < 0 || port >= 4) {
        return nullptr;
    }
    return mDoCPd_c::m_gamePad[port];
}

void JUTGamePad::CRumble::setEnabled(u32 mask) {
    mEnabled = mask;
}

void JUTGamePad::CButton::update(const PADStatus* padStatus, u32 stickStatus) {
    u32 buttons = stickStatus | (padStatus != nullptr ? padStatus->button : 0);
    mTrigger = buttons & ~mButton;
    mRelease = mButton & ~buttons;
    mButton = buttons;
    mRepeat = mTrigger;

    if (padStatus != nullptr) {
        mAnalogA = padStatus->analogA;
        mAnalogB = padStatus->analogB;
        mAnalogL = padStatus->triggerLeft;
        mAnalogR = padStatus->triggerRight;
    } else {
        mAnalogA = 0;
        mAnalogB = 0;
        mAnalogL = 0;
        mAnalogR = 0;
    }

    mAnalogLf = static_cast<f32>(mAnalogL) / 150.0f;
    mAnalogRf = static_cast<f32>(mAnalogR) / 150.0f;
}

u32 JUTGamePad::CStick::update(s8 x, s8 y, JUTGamePad::EStickMode mode,
                               JUTGamePad::EWhichStick stick, u32 buttons) {
    const s32 clamp = stick == EMainStick ? 54 : 42;

    mRawX = x;
    mRawY = y;
    mPosX = static_cast<f32>(x) / static_cast<f32>(clamp);
    mPosY = static_cast<f32>(y) / static_cast<f32>(clamp);
    mValue = std::sqrt((mPosX * mPosX) + (mPosY * mPosY));

    if (mValue > 1.0f) {
        if (mode == EStickMode1) {
            mPosX /= mValue;
            mPosY /= mValue;
        }
        mValue = 1.0f;
    }

    if (mValue > 0.0f) {
        if (mPosY == 0.0f) {
            mAngle = mPosX > 0.0f ? 0x4000 : -0x4000;
        } else {
            mAngle = static_cast<s16>((0x8000 / 3.1415926f) * std::atan2(mPosX, -mPosY));
        }
    } else {
        mAngle = 0;
    }

    return getButton(buttons >> (stick == EMainStick ? 24 : 16));
}

u32 JUTGamePad::CStick::getButton(u32 buttons) {
    u32 button = buttons & (PAD_BUTTON_UP | PAD_BUTTON_DOWN | PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT);

    if (-sReleasePoint < mPosX && mPosX < sReleasePoint) {
        button &= ~(PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT);
    } else if (mPosX <= -sPressPoint) {
        button &= ~PAD_BUTTON_RIGHT;
        button |= PAD_BUTTON_LEFT;
    } else if (mPosX >= sPressPoint) {
        button &= ~PAD_BUTTON_LEFT;
        button |= PAD_BUTTON_RIGHT;
    }

    if (-sReleasePoint < mPosY && mPosY < sReleasePoint) {
        button &= ~(PAD_BUTTON_UP | PAD_BUTTON_DOWN);
    } else if (mPosY <= -sPressPoint) {
        button &= ~PAD_BUTTON_UP;
        button |= PAD_BUTTON_DOWN;
    } else if (mPosY >= sPressPoint) {
        button &= ~PAD_BUTTON_DOWN;
        button |= PAD_BUTTON_UP;
    }

    return button;
}

// ---------------------------------------------------------------------------
// mDoGph_ graphics stubs (referenced from c_API.cpp function pointer table)
// ---------------------------------------------------------------------------
#ifndef TP_PORT_USE_REAL_MDO_GRAPHIC
// c_API.cpp forward-declares mDoGph_Create() as returning void (wrong), while
// m_Do_graphic.h (and m_Do_main.cpp) declare it as returning int.  On MSVC
// the return type is encoded in the mangled name, producing two distinct
// symbols.  Define the int version and alias the void symbol to it so both
// TUs link without changing any game source.
static void InitPortRenderTargets() {
    gPortFrameBufferTimg.format = GX_TF_RGB5A3;
    gPortFrameBufferTimg.width = FB_WIDTH / 2;
    gPortFrameBufferTimg.height = FB_HEIGHT / 2;
    gPortFrameBufferTimg.minFilter = GX_LINEAR;
    gPortFrameBufferTimg.magFilter = GX_LINEAR;
    gPortFrameBufferTimg.mipmapCount = 1;
    gPortFrameBufferTimg.imageOffset = 0;

    gPortZBufferTimg = gPortFrameBufferTimg;
    gPortWideFrameBufferTimg = gPortFrameBufferTimg;
    gPortWideFrameBufferTimg.width = FB_WIDTH;
    gPortWideFrameBufferTimg.height = FB_HEIGHT;
}

int mDoGph_Create() {
    tp::log::info("mDoGph_Create: begin");
    if (JFWDisplay::getManager() == nullptr) {
        tp::log::info("mDoGph_Create: creating JFWDisplay manager");
        JFWDisplay::createManager(nullptr, nullptr, 2, false);
    }

    InitPortRenderTargets();
    tp::log::info("mDoGph_Create: render targets initialized");

    mDoGph_gInf_c::mFader = &gPortFader;
    JFWDisplay::getManager()->setFader(mDoGph_gInf_c::mFader);
    tp::log::info("mDoGph_Create: fader installed");

    mDoGph_gInf_c::mFrameBufferTimg = &gPortFrameBufferTimg;
    mDoGph_gInf_c::mFrameBufferTex = gPortFrameBufferPixels;
    mDoGph_gInf_c::mZbufferTimg = &gPortZBufferTimg;
    mDoGph_gInf_c::mZbufferTex = gPortZBufferPixels;
#if WIDESCREEN_SUPPORT
    mDoGph_gInf_c::m_fullFrameBufferTimg = &gPortWideFrameBufferTimg;
    mDoGph_gInf_c::m_fullFrameBufferTex = gPortWideFrameBufferPixels;
#endif
    mDoGph_gInf_c::mFadeColor = mDoGph_gInf_c::mBackColor;
    mDoGph_gInf_c::mFadeRate = 0.0f;
    mDoGph_gInf_c::mFadeSpeed = 0.0f;
    mDoGph_gInf_c::mFade = 0;
    mDoGph_gInf_c::mBlureFlag = 0;
    mDoGph_gInf_c::mBlureRate = 0;
    tp::log::info("mDoGph_Create: complete");
    return 1;
}
#ifdef _MSC_VER
// ?mDoGph_Create@@YAXXZ  = void __cdecl mDoGph_Create(void)
// ?mDoGph_Create@@YAHXZ  = int  __cdecl mDoGph_Create(void)
#pragma comment(linker, "/alternatename:?mDoGph_Create@@YAXXZ=?mDoGph_Create@@YAHXZ")
#endif
void mDoGph_BeforeOfDraw() {}

void mDoGph_AfterOfDraw() {
    if (JFWDisplay::getManager() != nullptr) {
        JFWDisplay::getManager()->endRender();
    }
    tp::window::EndFrame();
}

void mDoGph_Painter() {
    if (!tp::window::IsOpen()) {
        tp::log::warn("mDoGph_Painter: window closed, terminating port");
        std::exit(0);
    }

    tp::window::BeginFrame();
    mDoGph_gInf_c::beginRender();

    ++gPortPresentedFrames;
    if (gPortPresentedFrames <= 3) {
        tp::log::info("mDoGph_Painter: host frame %u", gPortPresentedFrames);
    }
}
void mDoGph_BlankingON()  {}
void mDoGph_BlankingOFF() {}
void mDoGph_drawFilterQuad(s8, s8) {}

void mDoGph_gInf_c::create() {
    mDoGph_Create();
}

void mDoGph_gInf_c::beginRender() {
    if (JFWDisplay::getManager() != nullptr) {
        JFWDisplay::getManager()->beginRender();
    }
}

void mDoGph_gInf_c::fadeOut(f32 fadeSpeed, _GXColor& fadeColor) {
    mFade = 1;
    mFadeSpeed = fadeSpeed;
    mFadeRate = fadeSpeed > 0.0f ? 1.0f : 0.0f;
    mFadeColor = fadeColor;
    if (mFader != nullptr) {
        mFader->setColor(*reinterpret_cast<JUtility::TColor*>(&fadeColor));
    }
}

void mDoGph_gInf_c::fadeOut(f32 fadeSpeed) {
    fadeOut(fadeSpeed, mBackColor);
}

void mDoGph_gInf_c::fadeOut_f(f32 fadeSpeed, _GXColor& fadeColor) {
    fadeOut(fadeSpeed, fadeColor);
}

void mDoGph_gInf_c::onBlure() {
    mBlureFlag = 1;
}

void mDoGph_gInf_c::onBlure(const Mtx m) {
    cMtx_copy(m, mBlureMtx);
    onBlure();
}

void mDoGph_gInf_c::calcFade() {
    mFade = 0;
}

#if PLATFORM_WII || PLATFORM_SHIELD
bool mDoGph_gInf_c::csr_c::isPointer() {
    return false;
}

void mDoGph_gInf_c::csr_c::particleExecute() {}

void mDoGph_gInf_c::entryBaseCsr(csr_c* csr) {
    m_baseCsr = csr;
}

void mDoGph_gInf_c::releaseCsr(void) {
    m_csr = nullptr;
}

void mDoGph_gInf_c::entryCsr(csr_c* csr) {
    m_csr = csr;
}
#endif

#if WIDESCREEN_SUPPORT
void mDoGph_gInf_c::setTvSize() {}

void mDoGph_gInf_c::onWide() {
    mWide = 1;
}

void mDoGph_gInf_c::offWide() {
    mWide = 0;
}

u8 mDoGph_gInf_c::isWide() {
    return mWide;
}

void mDoGph_gInf_c::onWideZoom() {
    mWideZoom = 1;
}

void mDoGph_gInf_c::offWideZoom() {
    mWideZoom = 0;
}

BOOL mDoGph_gInf_c::isWideZoom() {
    return mWideZoom;
}

void mDoGph_gInf_c::setWideZoomProjection(Mtx44& m) {
    (void)m;
}

void mDoGph_gInf_c::setWideZoomLightProjection(Mtx& m) {
    (void)m;
}
#endif

mDoGph_gInf_c::bloom_c mDoGph_gInf_c::m_bloom = {};
GXTexObj mDoGph_gInf_c::mFrameBufferTexObj = {};
GXTexObj mDoGph_gInf_c::mZbufferTexObj = {};
Mtx mDoGph_gInf_c::mBlureMtx = {};
GXColor mDoGph_gInf_c::mBackColor = {0, 0, 0, 0};
GXColor mDoGph_gInf_c::mFadeColor = {0, 0, 0, 0};
JUTFader* mDoGph_gInf_c::mFader = nullptr;
ResTIMG* mDoGph_gInf_c::mFrameBufferTimg = nullptr;
void* mDoGph_gInf_c::mFrameBufferTex = nullptr;
ResTIMG* mDoGph_gInf_c::mZbufferTimg = nullptr;
void* mDoGph_gInf_c::mZbufferTex = nullptr;
f32 mDoGph_gInf_c::mFadeRate = 0.0f;
f32 mDoGph_gInf_c::mFadeSpeed = 0.0f;
u8 mDoGph_gInf_c::mBlureFlag = 0;
u8 mDoGph_gInf_c::mBlureRate = 0;
u8 mDoGph_gInf_c::mFade = 0;
bool mDoGph_gInf_c::mAutoForcus = false;

#if PLATFORM_WII || PLATFORM_SHIELD
u32 mDoGph_gInf_c::csr_c::m_blurID = 0;
cXyz mDoGph_gInf_c::csr_c::m_oldEffPos = cXyz::Zero;
cXyz mDoGph_gInf_c::csr_c::m_oldOldEffPos = cXyz::Zero;
cXyz mDoGph_gInf_c::csr_c::m_nowEffPos = cXyz::Zero;
mDoGph_gInf_c::csr_c* mDoGph_gInf_c::m_baseCsr = nullptr;
mDoGph_gInf_c::csr_c* mDoGph_gInf_c::m_csr = nullptr;
cXyz mDoGph_gInf_c::m_nowEffPos = cXyz::Zero;
cXyz mDoGph_gInf_c::m_oldEffPos = cXyz::Zero;
cXyz mDoGph_gInf_c::m_oldOldEffPos = cXyz::Zero;
#endif

#if WIDESCREEN_SUPPORT
GXTexObj mDoGph_gInf_c::m_fullFrameBufferTexObj = {};
u8 mDoGph_gInf_c::mWide = 1;
u8 mDoGph_gInf_c::mWideZoom = 0;
ResTIMG* mDoGph_gInf_c::m_fullFrameBufferTimg = nullptr;
void* mDoGph_gInf_c::m_fullFrameBufferTex = nullptr;
f32 mDoGph_gInf_c::m_aspect = 608.0f / 448.0f;
f32 mDoGph_gInf_c::m_scale = 1.0f;
f32 mDoGph_gInf_c::m_invScale = 1.0f;
f32 mDoGph_gInf_c::m_minXF = 0.0f;
f32 mDoGph_gInf_c::m_minYF = 0.0f;
int mDoGph_gInf_c::m_minX = 0;
int mDoGph_gInf_c::m_minY = 0;
f32 mDoGph_gInf_c::m_maxXF = FB_WIDTH - 1.0f;
f32 mDoGph_gInf_c::m_maxYF = FB_HEIGHT - 1.0f;
int mDoGph_gInf_c::m_maxX = FB_WIDTH - 1;
int mDoGph_gInf_c::m_maxY = FB_HEIGHT - 1;
int mDoGph_gInf_c::m_width = FB_WIDTH;
int mDoGph_gInf_c::m_height = FB_HEIGHT;
f32 mDoGph_gInf_c::m_heightF = FB_HEIGHT;
f32 mDoGph_gInf_c::m_widthF = FB_WIDTH;
#endif
#endif

// ---------------------------------------------------------------------------
// m_Do_audio stubs
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// DynamicLink / REL stubs
// ---------------------------------------------------------------------------
#include "c/c_dylink.h"

// ---------------------------------------------------------------------------
// m_Do_dvd_thread non-debug statics
// ---------------------------------------------------------------------------
#include "m_Do/m_Do_dvd_thread.h"
u8 mDoDvdThd::verbose = 0;
u8 mDoDvdThd::DVDLogoMode = 0;
u8 mDoDvdThd::Report_DVDRead = 0;

// ---------------------------------------------------------------------------
// f_ap_game stubs — fapGm_Create/Execute/g_HIO/ctor now provided by
// src/f_ap/f_ap_game.cpp.  Only the static members that the real file
// gates behind #if DEBUG need to be supplied here for non-debug builds.
// ---------------------------------------------------------------------------

// JFWSystem static data
// ---------------------------------------------------------------------------
JKRExpHeap* JFWSystem::rootHeap = nullptr;
JKRExpHeap* JFWSystem::systemHeap = nullptr;
JKRThread* JFWSystem::mainThread = nullptr;
JUTDbPrint* JFWSystem::debugPrint = nullptr;
JUTResFont* JFWSystem::systemFont = nullptr;
JUTConsoleManager* JFWSystem::systemConsoleManager = nullptr;
JUTConsole* JFWSystem::systemConsole = nullptr;
bool JFWSystem::sInitCalled = false;
s32 JFWSystem::CSetUpParam::maxStdHeaps = 1;
u32 JFWSystem::CSetUpParam::sysHeapSize = 0;
u32 JFWSystem::CSetUpParam::fifoBufSize = 0;
u32 JFWSystem::CSetUpParam::aramAudioBufSize = 0;
u32 JFWSystem::CSetUpParam::aramGraphBufSize = 0;
s32 JFWSystem::CSetUpParam::streamPriority = 0;
s32 JFWSystem::CSetUpParam::decompPriority = 0;
s32 JFWSystem::CSetUpParam::aPiecePriority = 0;
ResFONT* JFWSystem::CSetUpParam::systemFontRes = nullptr;
const GXRenderModeObj* JFWSystem::CSetUpParam::renderMode = nullptr;
u32 JFWSystem::CSetUpParam::exConsoleBufferSize = 0;

JUTException* JUTException::sErrorManager = nullptr;
JUTExceptionUserCallback JUTException::sPreUserCallback = nullptr;
JUTExceptionUserCallback JUTException::sPostUserCallback = nullptr;
void* JUTException::sConsoleBuffer = nullptr;
u32 JUTException::sConsoleBufferSize = 0;
JUTConsole* JUTException::sConsole = nullptr;

// ---------------------------------------------------------------------------
// JUTConsole C ABI helpers
// ---------------------------------------------------------------------------
static JUTConsole* sReportConsole = nullptr;
static JUTConsole* sWarningConsole = nullptr;

extern "C" void JUTConsole_print_f_va_(JUTConsole* /*console*/, const char* fmt, va_list args) {
    if (fmt) {
        ::vprintf(fmt, args);
    }
}

extern "C" void JUTSetReportConsole(JUTConsole* console) {
    sReportConsole = console;
}

extern "C" JUTConsole* JUTGetReportConsole() {
    return sReportConsole;
}

extern "C" void JUTSetWarningConsole(JUTConsole* console) {
    sWarningConsole = console;
}

extern "C" JUTConsole* JUTGetWarningConsole() {
    return sWarningConsole;
}

extern "C" void JUTWarningConsole_f_va(const char* fmt, va_list args) {
    if (fmt) {
        ::vprintf(fmt, args);
    }
}

extern "C" void JUTReportConsole_f_va(const char* fmt, va_list args) {
    if (fmt) {
        ::vprintf(fmt, args);
    }
}

extern "C" void JUTReportConsole_f(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    JUTReportConsole_f_va(fmt, args);
    va_end(args);
}

extern "C" void JUTWarningConsole(const char* message) {
    if (message) {
        ::printf("%s", message);
    }
}

extern "C" void JUTWarningConsole_f(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    JUTWarningConsole_f_va(fmt, args);
    va_end(args);
}

extern "C" void JUTReportConsole(const char* message) {
    if (message) {
        ::printf("%s", message);
    }
}

// ---------------------------------------------------------------------------
// JUTGamePad C3ButtonReset static data
// ---------------------------------------------------------------------------
u32 JUTGamePad::C3ButtonReset::sResetPattern = 0;
u32 JUTGamePad::C3ButtonReset::sResetMaskPattern = 0;
callbackFn JUTGamePad::C3ButtonReset::sCallback = nullptr;
void* JUTGamePad::C3ButtonReset::sCallbackArg = nullptr;
OSTime JUTGamePad::C3ButtonReset::sThreshold = 0;
s32 JUTGamePad::C3ButtonReset::sResetOccurredPort = -1;
bool JUTGamePad::C3ButtonReset::sResetOccurred = false;
bool JUTGamePad::C3ButtonReset::sResetSwitchPushing = false;
JSUList<JUTGamePad> JUTGamePad::mPadList(false);
bool JUTGamePad::mListInitialized = false;
PADStatus JUTGamePad::mPadStatus[4] = {};
JUTGamePad::CButton JUTGamePad::mPadButton[4] = {};
JUTGamePad::CStick JUTGamePad::mPadMStick[4] = {};
JUTGamePad::CStick JUTGamePad::mPadSStick[4] = {};
JUTGamePad::EStickMode JUTGamePad::sStickMode = JUTGamePad::EStickMode1;
int JUTGamePad::sClampMode = JUTGamePad::EClampStick;
u8 JUTGamePad::mPadAssign[4] = {};
u32 JUTGamePad::sSuppressPadReset = 0;
s32 JUTGamePad::sAnalogMode = 0;
u32 JUTGamePad::sRumbleSupported = 0;
u32 JUTGamePad::CRumble::sChannelMask[4] = {0, 0, 0, 0};
u8 JUTGamePad::CRumble::mStatus[4] = {0, 0, 0, 0};
u32 JUTGamePad::CRumble::mEnabled = 0;
f32 JUTGamePad::CStick::sPressPoint = 0.5f;
f32 JUTGamePad::CStick::sReleasePoint = 0.25f;

JKRAram* JKRAram::sAramObject = nullptr;
u32 JKRAram::sSZSBufferSize = 0x400;
OSMessage JKRAram::sMessageBuffer[4] = {};
OSMessageQueue JKRAram::sMessageQueue = {};
JSUList<JKRAMCommand> JKRAram::sAramCommandList(false);
u32 JKRDvdRipper::sSZSBufferSize = 0x400;
u32 JKRDvdAramRipper::sSZSBufferSize = 0x400;
bool JKRDvdAramRipper::errorRetry = false;
JSUList<JKRADCommand> JKRDvdAramRipper::sDvdAramAsyncList(false);

void JKRAramStream::setTransBuffer(u8* /*buffer*/, u32 /*bufferSize*/, JKRHeap* /*heap*/) {}

// ---------------------------------------------------------------------------
// fapGm_HIO_c stubs — constructor + static members
// ---------------------------------------------------------------------------
// The constructor and all static members live in f_ap/f_ap_game.cpp which is
// not yet compiled for the port.  Provide the minimum needed to link.
#include "f_ap/f_ap_game.h"

// Static member definitions (declared in header unconditionally; game source
// only defines them under #if DEBUG, so we must provide them for non-debug).
u8            fapGm_HIO_c::m_CpuTimerOn         = 0;
u8            fapGm_HIO_c::m_CpuTimerOff        = 0;
u8            fapGm_HIO_c::m_CpuTimerStart      = 0;
u32           fapGm_HIO_c::m_CpuTimerTick       = 0;
CaptureScreen* fapGm_HIO_c::mCaptureScreen      = nullptr;
void*         fapGm_HIO_c::mCaptureScreenBuffer = nullptr;
s16           fapGm_HIO_c::mCaptureScreenFlag    = 0;
u16           fapGm_HIO_c::mCaptureScreenWidth   = 0;
u16           fapGm_HIO_c::mCaptureScreenHeight  = 0;
u16           fapGm_HIO_c::mCaptureScreenLinePf  = 0;
u16           fapGm_HIO_c::mCaptureScreenLineNum = 0;
u8            fapGm_HIO_c::mCaptureScreenNumH    = 0;
u8            fapGm_HIO_c::mCaptureScreenNumV    = 0;
u8            fapGm_HIO_c::mParticle254Fix       = 0;
u8            fapGm_HIO_c::mCaptureMagnification = 1;
u8            fapGm_HIO_c::mCaptureScreenDivH    = 1;
u8            fapGm_HIO_c::mCaptureScreenDivV    = 1;
u8            fapGm_HIO_c::mPackArchiveMode      = 1;

// fapGm_HIO_c constructor and g_HIO are now provided by
// src/f_ap/f_ap_game.cpp.  Removed from stubs.

// ---------------------------------------------------------------------------
// DynamicModuleControlBase stub
// ---------------------------------------------------------------------------
#include "DynamicLink.h"
void DynamicModuleControlBase::dump() {}

// ---------------------------------------------------------------------------
// mDoHIO stubs — non-debug definitions for mDoHIO_root_c / mDoHIO_subRoot_c.
// m_Do_hostIO.cpp defines these only inside #if DEBUG; provide no-op stubs
// for non-debug PC builds so the vtables link cleanly.
// ---------------------------------------------------------------------------
#include "m_Do/m_Do_hostIO.h"

mDoHIO_root_c mDoHIO_root;

mDoHIO_root_c::~mDoHIO_root_c() {}
mDoHIO_subRoot_c::~mDoHIO_subRoot_c() {}
mDoHIO_child_c::~mDoHIO_child_c() {}

void mDoHIO_root_c::genMessage(JORMContext* /*ctx*/) {}
void mDoHIO_subRoot_c::genMessage(JORMContext* /*ctx*/) {}

void mDoHIO_root_c::update() {}
void mDoHIO_root_c::updateChild(s8 /*no*/) {}
void mDoHIO_root_c::deleteChild(s8 /*no*/) {}

s8 mDoHIO_subRoot_c::createChild(const char* /*name*/, JORReflexible* /*node*/) { return -1; }
void mDoHIO_subRoot_c::deleteChild(s8 /*no*/) {}
void mDoHIO_subRoot_c::updateChild(s8 /*no*/) {}

void mDoHIO_deleteChild(s8 /*no*/) {}
void mDoHIO_updateChild(s8 /*no*/) {}

// ---------------------------------------------------------------------------
// mDoDvdHack — DVD entry-number-to-name mapping (no DVD filesystem on PC)
// ---------------------------------------------------------------------------
#include "m_Do/m_Do_dvd_thread.h"
namespace mDoDvdHack {
    const char* ConvertEntrynumToName(s32 /*entry*/) { return "<dvd-entry>"; }
}
