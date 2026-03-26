/**
 * port/src/m_Do/m_Do_stubs.cpp
 * PC port stubs for m_Do subsystem symbols needed by SComponent files.
 * The full m_Do_graphic / m_Do_controller_pad implementations have too many
 * GC-specific dependencies to compile on PC; this file supplies the
 * minimum set of symbols that c_API*.cpp pulls in at link time.
 */
#include "m_Do/m_Do_controller_pad.h"

// ---------------------------------------------------------------------------
// mDoCPd_c static data
// ---------------------------------------------------------------------------
JUTGamePad* mDoCPd_c::m_gamePad[4]       = {};
interface_of_controller_pad mDoCPd_c::m_cpadInfo[4]      = {};
interface_of_controller_pad mDoCPd_c::m_debugCpadInfo[4] = {};

// ---------------------------------------------------------------------------
// mDoCPd_c methods
// ---------------------------------------------------------------------------
void mDoCPd_c::create()       {}
void mDoCPd_c::read()         {}
void mDoCPd_c::recalibrate()  {}
void mDoCPd_c::convert(interface_of_controller_pad* /*dst*/, JUTGamePad* /*src*/) {}
void mDoCPd_c::LRlockCheck(interface_of_controller_pad* /*p*/) {}

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
// m_Do_ext heap globals (normally set up by mDoMch_Create / m_Do_ext.cpp)
// ---------------------------------------------------------------------------
#include "JSystem/JKernel/JKRHeap.h"
JKRExpHeap* zeldaHeap   = nullptr;
JKRExpHeap* gameHeap    = nullptr;
JKRExpHeap* archiveHeap = nullptr;

// ---------------------------------------------------------------------------
// m_Do_audio stubs
// ---------------------------------------------------------------------------
#include "JSystem/JKernel/JKRHeap.h"
JKRSolidHeap* g_mDoAud_audioHeap = nullptr;

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
// m_Do_machine stubs
// ---------------------------------------------------------------------------
int  mDoMch_Create()       { return 1; }
void mDoMch_HeapCheckAll() {}

// ---------------------------------------------------------------------------
// m_Do_dvd_thread stubs
// ---------------------------------------------------------------------------
#include "m_Do/m_Do_dvd_thread.h"
bool mDoDvdThd::SyncWidthSound = false;
mDoDvdThd_callback_c* mDoDvdThd_callback_c::create(mDoDvdThd_callback_func /*f*/, void* /*arg*/) { return nullptr; }

// ---------------------------------------------------------------------------
// f_ap_game stubs — fapGm_Create/Execute/g_HIO/ctor now provided by
// src/f_ap/f_ap_game.cpp.  Only the static members that the real file
// gates behind #if DEBUG need to be supplied here for non-debug builds.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// m_Do_Reset static data
// ---------------------------------------------------------------------------
#include "m_Do/m_Do_Reset.h"
mDoRstData* mDoRst::mResetData = nullptr;

// ---------------------------------------------------------------------------
// m_Do_MemCard stubs
// ---------------------------------------------------------------------------
#include "m_Do/m_Do_MemCard.h"
mDoMemCd_Ctrl_c::mDoMemCd_Ctrl_c() {}
mDoMemCd_Ctrl_c g_mDoMemCd_control;
void mDoMemCd_Ctrl_c::update() {}

// ---------------------------------------------------------------------------
// JFWSystem static data
// ---------------------------------------------------------------------------
#include "JSystem/JFramework/JFWSystem.h"
JUTConsole* JFWSystem::systemConsole = nullptr;

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
