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
int  mDoGph_Create()      { return 1; }
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
// f_ap_game stubs (framework create/execute — no-ops until f_ap compiles)
// ---------------------------------------------------------------------------
void fapGm_Create()  {}
void fapGm_Execute() {}

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
// HIO global — m_Do_main.cpp declares `extern fapGm_HIO_c g_HIO` so the
// type must match exactly; fapGm_HIO_c is now concrete (genMessage no-op).
// ---------------------------------------------------------------------------
#include "f_ap/f_ap_game.h"
fapGm_HIO_c g_HIO;

// ---------------------------------------------------------------------------
// DynamicModuleControlBase stub
// ---------------------------------------------------------------------------
#include "DynamicLink.h"
void DynamicModuleControlBase::dump() {}
