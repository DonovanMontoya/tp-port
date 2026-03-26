#pragma once
/**
 * port/include/d/d_com_inf_game.h
 * PC port stub — minimal declarations for d_com_inf_game.
 * Provides just enough for m_Do_main.cpp and f_op_actor.cpp to compile.
 */
#ifndef D_COM_D_COM_INF_GAME_H
#define D_COM_D_COM_INF_GAME_H

#include "port/types.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "SSystem/SComponent/c_xyz.h"
#include "SSystem/SComponent/c_lib.h"
#include "d/d_event.h"
#include "d/d_event_manager.h"
#include "d/d_drawlist.h"
#include "Z2AudioLib/Z2SeMgr.h"
#include "dolphin/gx/GXStruct.h"

// Minimal resource control stub
struct dResCont_c {
    void dump() {}
};

// Minimal game info struct
struct dComIfG_gameInfo_c {
    dResCont_c mResControl;
};

extern dComIfG_gameInfo_c g_dComIfG_gameInfo;

// Forward declarations for types used in inline stubs below
class csXyz;
struct JPABaseEmitter;
class dKy_tevstr_c;
class dPa_levelEcallBack;
struct _GXColor;
class camera_process_class;
struct dStage_roomDt_c;

// Free functions
inline void dComIfG_dumpResControl() {}
inline void dComIfG_ct() {}
// Return -1 = no active room (avoids null-deref in room data path)
inline int  dComIfGp_roomControl_getStayNo() { return -1; }
inline u8   dComIfGp_isPauseFlag() { return 0; }

// Event / move system — backed by a static stub instance (never runs on PC)
namespace port_detail {
    inline dEvt_control_c& getEventStub()  { static dEvt_control_c s; return s; }
    inline dEvent_manager_c& getEvtMgrStub() { static dEvent_manager_c s; return s; }
}
inline dEvt_control_c*   dComIfGp_getEvent()        { return &port_detail::getEventStub(); }
inline dEvent_manager_c& dComIfGp_getEventManager() { return port_detail::getEvtMgrStub(); }

inline int   dComIfGp_event_moveApproval(void* /*actor*/) { return 2; }
inline BOOL  dComIfGp_event_runCheck()                    { return FALSE; }
inline f32   dComIfGp_event_getCullRate()                 { return 1.0f; }
inline int   dComIfGp_event_order(u16 /*type*/, u16 /*prio*/, u16 /*flags*/, u16 /*hind*/,
                                   void* /*req*/, void* /*tgt*/, s16 /*evId*/, u8 /*toolId*/) {
    return -1;
}
inline fopAc_ac_c* dComIfGp_event_getPt1()           { return nullptr; }
inline fopAc_ac_c* dComIfGp_event_getPt2()           { return nullptr; }
inline fopAc_ac_c* dComIfGp_event_getTalkPartner()   { return nullptr; }
inline fopAc_ac_c* dComIfGp_event_getItemPartner()   { return nullptr; }
inline void        dComIfGp_event_setGtItm(u8)       {}

// Player/camera accessors
class fopAc_ac_c;
inline fopAc_ac_c*           dComIfGp_getPlayer(int /*idx*/) { return nullptr; }
inline camera_process_class* dComIfGp_getCamera(int /*idx*/) { return nullptr; }

// Room control
inline dStage_roomDt_c* dComIfGp_roomControl_getStatusRoomDt(int /*room_no*/) { return nullptr; }

// Game switch/flag queries (never any switches set on PC)
inline BOOL dComIfGs_isSwitch(int /*no*/, int /*roomNo*/) { return FALSE; }
// Game state life getters
inline u16 dComIfGs_getLife()    { return 0; }
inline u16 dComIfGs_getMaxLife() { return 0; }
// Item table (no data on PC)
inline void* dComIfGp_getItemTable() { return nullptr; }
// Start stage name
inline const char* dComIfGp_getStartStageName() { return ""; }
// Room control helpers
inline bool dComIfGp_roomControl_checkStatusFlag(int /*roomNo*/, u8 /*flag*/) { return false; }

// Particle stubs — all return nullptr / 0 / no-op on PC
// u32-handle variants (particles tracked by handle)
inline u32 dComIfGp_particle_set(u32 /*h*/, u16 /*id*/, const cXyz* /*pos*/,
                                  const csXyz* /*rot*/, const cXyz* /*scale*/) { return 0; }
inline u32 dComIfGp_particle_set(u32 /*h*/, u16 /*id*/, const cXyz* /*pos*/,
                                  const dKy_tevstr_c* /*tev*/) { return 0; }
// JPABaseEmitter* variants
inline JPABaseEmitter* dComIfGp_particle_set(u16, const cXyz*, const csXyz*, const cXyz*) {
    return nullptr;
}
inline JPABaseEmitter* dComIfGp_particle_set(u16, const cXyz*, const dKy_tevstr_c*,
                                              const csXyz*, const cXyz*) {
    return nullptr;
}
// Variant used by fopEn_enemy_c ball model effects
inline JPABaseEmitter* dComIfGp_particle_set(u16 /*id*/, u16 /*efxId*/,
                                              const cXyz* /*pos*/, const dKy_tevstr_c* /*tev*/) {
    return nullptr;
}
inline void dComIfGp_particle_setSimple(u16, const cXyz*, u8, _GXColor&, float) {}
inline void dComIfGp_particle_setSimple(u16, const cXyz*, u8, _GXColor&, float,
                                         dPa_levelEcallBack*) {}
inline void dComIfGp_drawSimpleModel() {}
inline void dComIfGd_peekZdata() {}
// Particle emitter getters
inline JPABaseEmitter* dComIfGp_particle_getEmitter(u32 /*handle*/) { return nullptr; }
// Foot particle — variadic signature (many params); just return 0
struct cBgS_PolyInfo;
inline u32 dComIfGp_particle_setSimpleFoot(u32 /*h*/, u32* /*out*/, cBgS_PolyInfo& /*bi*/,
                                            const cXyz* /*pos*/, const dKy_tevstr_c* /*tev*/,
                                            int /*flg*/, const csXyz* /*rot*/,
                                            const cXyz* /*scale*/, void* /*p9*/,
                                            u8 /*alpha*/, void* /*p11*/) { return 0; }

// Resource access — returns nullptr on PC (no game data loaded yet)
inline void* dComIfG_getObjectRes(const char* /*name*/, int /*idx*/) { return nullptr; }

// (dComIfG_Bgsp is declared in port/include/d/d_bg_s.h)

// m_Do_ext heap globals/accessors (defined in m_Do_ext.cpp / port stubs)
extern JKRExpHeap* zeldaHeap;
extern JKRExpHeap* gameHeap;
extern JKRExpHeap* archiveHeap;
inline JKRExpHeap* mDoExt_getZeldaHeap()   { return zeldaHeap; }
inline JKRExpHeap* mDoExt_getGameHeap()    { return gameHeap; }
inline JKRExpHeap* mDoExt_getArchiveHeap() { return archiveHeap; }
inline JKRExpHeap* mDoExt_getJ2dHeap()     { return nullptr; }
inline JKRExpHeap* mDoExt_getHostIOHeap()   { return nullptr; }
inline JKRExpHeap* mDoExt_getCommandHeap() { return nullptr; }
inline JKRHeap*    mDoExt_setCurrentHeap(JKRHeap* h) { return JKRSetCurrentHeap(h); }
inline JKRExpHeap* mDoExt_getAssertHeap()  { return nullptr; }

// f_op actor manager init — defined in f_op_actor_mng.cpp
void fopAcM_initManager();

// Audio (defined in m_Do_audio.cpp / port stubs)
extern JKRSolidHeap* g_mDoAud_audioHeap;
inline void mDoAud_Execute() {}

// -----------------------------------------------------------------------
// Message / heap lock stubs — used by f_op_msg_mng.cpp
// -----------------------------------------------------------------------
// Returns 0 = "free" so message system proceeds normally
inline u8 dComIfGp_isHeapLockFlag() { return 0; }

// Message animation / camera tag info — no-ops on PC (no message UI)
inline void dComIfGp_clearMesgAnimeTagInfo()  {}
inline void dComIfGp_clearMesgCameraTagInfo() {}

// Event register — no-op on PC
inline void dComIfGs_setEventReg(u16 /*reg*/, u8 /*val*/) {}

// Message object class accessor — forward-declared here; the real class is
// in d/d_msg_object.h (which includes this file via dolzel.h).
class dMsgObject_c;
inline dMsgObject_c* dComIfGp_getMsgObjectClass() { return nullptr; }

// Attention system accessor — returns nullptr; dAttention_c defined in d_camera.h
class dAttention_c;
inline dAttention_c* dComIfGp_getAttention() { return nullptr; }

// 2D draw list submission — no-op on PC (no J2DGraph rendering)
inline void dComIfGd_set2DOpa(dDlst_base_c* /*dlst*/)    {}
inline void dComIfGd_set2DXlu(dDlst_base_c* /*dlst*/)    {}
inline void dComIfGd_set2DOpaTop(dDlst_base_c* /*dlst*/) {}
inline void dComIfGd_setCopy2D(dDlst_base_c* /*dlst*/)   {}

// Resource manager stubs
inline int  dComIfG_syncAllObjectRes()                              { return 0; }
inline int  dComIfG_setObjectRes(const char*, u8, JKRHeap*)        { return 0; }
inline int  dComIfG_setObjectRes(const char*, void*, u32, JKRHeap*){ return 0; }
inline int  dComIfG_deleteObjectResMain(const char*)               { return 0; }
inline int  dComIfG_changeOpeningScene(void*, s16)                 { return 0; }

// Phase handler — always "done" on PC
struct request_of_phase_process_class;
typedef int (*request_of_phase_process_fn)(void*);
inline int dComLbG_PhaseHandler(request_of_phase_process_class*, request_of_phase_process_fn*, void*) { return 1; }

// Particle resource heap
inline JKRExpHeap* dComIfGp_particle_getResHeap() { return nullptr; }

// Scene change request
class scene_class;
inline int fopScnM_ChangeReq(scene_class*, s16, s16, u16) { return 0; }

// -----------------------------------------------------------------------
// Resource archive setters — all no-ops on PC (no DVD/ARAM loading)
// -----------------------------------------------------------------------
class JKRArchive;
inline void dComIfGp_setFieldMapArchive2(JKRArchive*)    {}
inline void dComIfGp_setAnmArchive(JKRArchive*)          {}
inline void dComIfGp_setFmapResArchive(JKRArchive*)      {}
inline void dComIfGp_setDmapResArchive(JKRArchive*)      {}
inline void dComIfGp_setCollectResArchive(JKRArchive*)   {}
inline void dComIfGp_setItemIconArchive(JKRArchive*)     {}
inline void dComIfGp_setAllMapArchive(JKRArchive*)       {}
inline void dComIfGp_setRingResArchive(JKRArchive*)      {}
inline void dComIfGp_setNameResArchive(JKRArchive*)      {}
inline void dComIfGp_setDemoMsgArchive(JKRArchive*)      {}
inline void dComIfGp_setMeterButtonArchive(JKRArchive*)  {}
inline void dComIfGp_setErrorResArchive(JKRArchive*)     {}
inline void dComIfGp_setCardIconResArchive(JKRArchive*)  {}
inline void dComIfGp_setMsgDtArchive(int, JKRArchive*)   {}
inline void dComIfGp_setMsgCommonArchive(JKRArchive*)    {}
inline void dComIfGp_setMsgArchive(int, JKRArchive*)     {}
inline void dComIfGp_setFontArchive(JKRArchive*)         {}
inline void dComIfGp_setRubyArchive(JKRArchive*)         {}
inline void dComIfGp_setMain2DArchive(JKRArchive*)       {}
inline void dComIfGp_setItemTable(void*)                 {}

// Particle creation — no-op on PC
inline void dComIfGp_particle_create()                   {}
inline void dComIfGp_particle_createCommon(void*)        {}

// J3DDrawBuffer accessors — stub returns nullptr (no rendering on PC)
inline J3DDrawBuffer* dComIfGd_getOpaListBG()            { return nullptr; }
inline J3DDrawBuffer* dComIfGd_getOpaList()              { return nullptr; }
inline J3DDrawBuffer* dComIfGd_getXluList()              { return nullptr; }
inline J3DDrawBuffer* dComIfGd_getOpaListFilter()        { return nullptr; }

// Black color global (used for fade effects)
extern GXColor g_blackColor;

#endif /* D_COM_D_COM_INF_GAME_H */
