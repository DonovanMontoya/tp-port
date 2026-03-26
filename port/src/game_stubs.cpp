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
#include "JSystem/J3DGraphBase/J3DSys.h"
#include "m_Do/m_Do_mtx.h"
#include "m_Do/m_Do_ext.h"

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

// ---------------------------------------------------------------------------
// mDoMtx_stack_c statics (m_Do/m_Do_mtx.h port stub)
// ---------------------------------------------------------------------------
Mtx  mDoMtx_stack_c::now;
Mtx  mDoMtx_stack_c::buffer[16];
Mtx* mDoMtx_stack_c::next;
Mtx* mDoMtx_stack_c::end;

// ---------------------------------------------------------------------------
// DummyCheckHeap global (m_Do/m_Do_ext.h port stub)
// ---------------------------------------------------------------------------
DummyCheckHeap* dch = nullptr;

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
// JASGlobalInstance specialization statics
// Z2SeMgr::sInstance is already defined in m_Do_main.cpp
// ---------------------------------------------------------------------------
#include "Z2AudioLib/Z2AudioMgr.h"
template<> Z2AudioMgr* JASGlobalInstance<Z2AudioMgr>::sInstance = nullptr;

// ---------------------------------------------------------------------------
// mDoAud_zelAudio_c static
// ---------------------------------------------------------------------------
#include "m_Do/m_Do_audio.h"
u8 mDoAud_zelAudio_c::mInitFlag = 0;

// ---------------------------------------------------------------------------
// JFWDisplay static
// ---------------------------------------------------------------------------
#include "JSystem/JFramework/JFWDisplay.h"
JFWDisplay* JFWDisplay::sManager = nullptr;

// ---------------------------------------------------------------------------
// JUTGamePad::C3ButtonReset statics
// ---------------------------------------------------------------------------
#include "JSystem/JUtility/JUTGamePad.h"
callbackFn JUTGamePad::C3ButtonReset::sCallback    = nullptr;
void*      JUTGamePad::C3ButtonReset::sCallbackArg = nullptr;
bool       JUTGamePad::C3ButtonReset::sResetOccurred = false;

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
