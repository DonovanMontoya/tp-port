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

// ---------------------------------------------------------------------------
// Actor manager stubs — defined in f_op_actor_mng.cpp (deferred)
// Provide minimal no-ops so f_op_actor.cpp links.
// ---------------------------------------------------------------------------
s32 fopAcM_cullingCheck(const fopAc_ac_c* /*actor*/) { return 0; }
s32 fopAcM_delete(fopAc_ac_c* /*actor*/)             { return 1; }
s32 fopAcM_delete(fpc_ProcID /*id*/)                 { return 1; }
void fopAcM_DeleteHeap(fopAc_ac_c* /*actor*/)        {}

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
