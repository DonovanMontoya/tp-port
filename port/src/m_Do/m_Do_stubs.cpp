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
void mDoGph_Create()      {}
void mDoGph_BeforeOfDraw(){}
void mDoGph_AfterOfDraw() {}
void mDoGph_Painter()     {}
void mDoGph_BlankingON()  {}
void mDoGph_BlankingOFF() {}
