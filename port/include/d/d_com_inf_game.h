#pragma once
/**
 * port/include/d/d_com_inf_game.h
 * PC port stub — minimal declarations for d_com_inf_game.
 * Provides just enough for m_Do_main.cpp and f_op_actor.cpp to compile.
 */
#ifndef D_COM_D_COM_INF_GAME_H
#define D_COM_D_COM_INF_GAME_H

#include "port/types.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "SSystem/SComponent/c_xyz.h"
#include "SSystem/SComponent/c_lib.h"

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
class JKRArchive;
class csXyz;
struct JPABaseEmitter;
class dKy_tevstr_c;
class dPa_levelEcallBack;
struct _GXColor;
typedef _GXColor GXColor;
class camera_process_class;
struct dStage_roomDt_c;
class dStage_startStage_c;

// Free functions
inline void dComIfG_dumpResControl() {}
inline void dComIfG_ct() {}
// Return -1 = no active room (avoids null-deref in room data path)
inline int  dComIfGp_roomControl_getStayNo() { return -1; }
inline u8   dComIfGp_isPauseFlag() { return 0; }

// Event / move system
inline int  dComIfGp_event_moveApproval(void* /*actor*/) { return 2; } // 2 = free move
inline BOOL dComIfGp_event_runCheck() { return FALSE; }

// Camera accessors
inline camera_process_class* dComIfGp_getCamera(int /*idx*/) { return nullptr; }

// Start stage accessors
inline dStage_startStage_c* dComIfGp_getStartStage() { return nullptr; }
inline void dComIfGp_setStartStage(dStage_startStage_c* /*p_startStage*/) {}
inline const char* dComIfGp_getStartStageName() { return "F_SP102"; }
inline s8 dComIfGp_getStartStageRoomNo() { return 0; }

// Room control
inline dStage_roomDt_c* dComIfGp_roomControl_getStatusRoomDt(int /*room_no*/) { return nullptr; }

// Game switch/flag queries (never any switches set on PC)
inline BOOL dComIfGs_isSwitch(int /*no*/, int /*roomNo*/) { return FALSE; }

// Particle stubs — all return nullptr / no-op on PC
inline u32 dComIfGp_particle_set(u32 /*key*/, u16 /*efxId*/, const cXyz* /*pos*/,
                                  const dKy_tevstr_c* /*tev*/, const csXyz* /*rot*/,
                                  const cXyz* /*scale*/, u8 /*alpha*/,
                                  dPa_levelEcallBack* /*cb*/, s8 /*roomNo*/,
                                  const GXColor* /*prim*/, const GXColor* /*env*/,
                                  const cXyz* /*particleScale*/) {
    return 0;
}
inline u32 dComIfGp_particle_set(u32 /*key*/, u16 /*efxId*/, const cXyz* /*pos*/,
                                  const csXyz* /*rot*/, const cXyz* /*scale*/,
                                  u8 /*alpha*/, dPa_levelEcallBack* /*cb*/,
                                  s8 /*roomNo*/, const GXColor* /*prim*/,
                                  const GXColor* /*env*/,
                                  const cXyz* /*particleScale*/) {
    return 0;
}
inline JPABaseEmitter* dComIfGp_particle_set(u16 /*efxId*/, const cXyz* /*pos*/,
                                              const dKy_tevstr_c* /*tev*/,
                                              const csXyz* /*rot*/,
                                              const cXyz* /*scale*/, u8 /*alpha*/,
                                              dPa_levelEcallBack* /*cb*/,
                                              s8 /*roomNo*/, const GXColor* /*prim*/,
                                              const GXColor* /*env*/,
                                              const cXyz* /*particleScale*/) {
    return nullptr;
}
inline JPABaseEmitter* dComIfGp_particle_set(u16 /*efxId*/, const cXyz* /*pos*/,
                                              const csXyz* /*rot*/,
                                              const cXyz* /*scale*/, u8 /*alpha*/,
                                              dPa_levelEcallBack* /*cb*/,
                                              s8 /*roomNo*/, const GXColor* /*prim*/,
                                              const GXColor* /*env*/,
                                              const cXyz* /*particleScale*/) {
    return nullptr;
}
inline JPABaseEmitter* dComIfGp_particle_set(u16, const cXyz*, const csXyz*, const cXyz*) {
    return nullptr;
}
inline JPABaseEmitter* dComIfGp_particle_set(u16, const cXyz*, const dKy_tevstr_c*,
                                              const csXyz*, const cXyz*) {
    return nullptr;
}
// Variant used by fopEn_enemy_c ball model effects
inline u32 dComIfGp_particle_set(u32 /*key*/, u16 /*efxId*/, const cXyz* /*pos*/,
                                  const dKy_tevstr_c* /*tev*/) {
    return 0;
}
inline u32 dComIfGp_particle_set(u32 /*key*/, u16 /*efxId*/, const cXyz* /*pos*/,
                                  const csXyz* /*rot*/, const cXyz* /*scale*/) {
    return 0;
}
inline void dComIfGp_particle_setSimple(u16, const cXyz*, u8, _GXColor&, float) {}
inline void dComIfGp_particle_setSimple(u16, const cXyz*, u8, _GXColor&, float,
                                         dPa_levelEcallBack*) {}
inline void dComIfGp_drawSimpleModel() {}
inline void dComIfGd_peekZdata() {}

// Resource access — returns nullptr on PC (no game data loaded yet)
inline void* dComIfG_getObjectRes(const char* /*name*/, int /*idx*/) { return nullptr; }

// m_Do_ext heap globals/accessors (defined in m_Do_ext.cpp / port stubs)
extern JKRExpHeap* zeldaHeap;
extern JKRExpHeap* gameHeap;
extern JKRExpHeap* archiveHeap;
JKRExpHeap* mDoExt_getZeldaHeap();
JKRExpHeap* mDoExt_getGameHeap();
JKRExpHeap* mDoExt_getArchiveHeap();
JKRExpHeap* mDoExt_getJ2dHeap();
inline JKRExpHeap* mDoExt_getHostIOHeap()   { return nullptr; }
JKRExpHeap* mDoExt_getCommandHeap();
inline JKRHeap*    mDoExt_setCurrentHeap(JKRHeap* h) { return JKRSetCurrentHeap(h); }

// f_op actor manager init (defined in f_op_actor_mng.cpp / port stubs)
inline void fopAcM_initManager() {}

// Audio (defined in m_Do_audio.cpp / port stubs)
extern JKRSolidHeap* g_mDoAud_audioHeap;
void mDoAud_Execute();

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
inline u32 dComIfGs_getPEventBit() { return 0; }

// Resource archives
inline JKRArchive* dComIfGp_getCardIconResArchive() {
    static JKRArchive archive;
    return &archive;
}

#endif /* D_COM_D_COM_INF_GAME_H */
