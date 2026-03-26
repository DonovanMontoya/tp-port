/**
 * port/src/m_Do/m_Do_stubs.cpp
 * PC port stubs for m_Do subsystem symbols needed by SComponent files.
 * The full m_Do_graphic / m_Do_controller_pad implementations have too many
 * GC-specific dependencies to compile on PC; this file supplies the
 * minimum set of symbols that c_API*.cpp pulls in at link time.
 */
#include <cstdarg>
#include <cstdio>

#include "m_Do/m_Do_controller_pad.h"
#include "JSystem/JFramework/JFWSystem.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTException.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "Z2AudioLib/Z2AudioMgr.h"
#include "Z2AudioLib/Z2SoundMgr.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "JSystem/JKernel/JKRAramStream.h"
#include "JSystem/JKernel/JKRDvdAramRipper.h"

template <>
Z2AudioMgr* JASGlobalInstance<Z2AudioMgr>::sInstance = nullptr;

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
}

JUTGamePad::~JUTGamePad() {}

u32 JUTGamePad::read() {
    return 0;
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

// ---------------------------------------------------------------------------
// mDoGph_ graphics stubs (referenced from c_API.cpp function pointer table)
// ---------------------------------------------------------------------------
// c_API.cpp forward-declares mDoGph_Create() as returning void (wrong), while
// m_Do_graphic.h (and m_Do_main.cpp) declare it as returning int.  On MSVC
// the return type is encoded in the mangled name, producing two distinct
// symbols.  Define the int version and alias the void symbol to it so both
// TUs link without changing any game source.
int  mDoGph_Create()      { return 1; }
#ifdef _MSC_VER
// ?mDoGph_Create@@YAXXZ  = void __cdecl mDoGph_Create(void)
// ?mDoGph_Create@@YAHXZ  = int  __cdecl mDoGph_Create(void)
#pragma comment(linker, "/alternatename:?mDoGph_Create@@YAXXZ=?mDoGph_Create@@YAHXZ")
#endif
void mDoGph_BeforeOfDraw(){}
void mDoGph_AfterOfDraw() {}
void mDoGph_Painter()     {}
void mDoGph_BlankingON()  {}
void mDoGph_BlankingOFF() {}

// ---------------------------------------------------------------------------
// m_Do_audio stubs
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// DynamicLink / REL stubs
// ---------------------------------------------------------------------------
#include "c/c_dylink.h"

// ---------------------------------------------------------------------------
// Game info global
// ---------------------------------------------------------------------------
#include "d/d_com_inf_game.h"
dComIfG_gameInfo_c g_dComIfG_gameInfo = {};

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
