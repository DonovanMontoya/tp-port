#pragma once
/**
 * port/include/d/d_com_inf_game.h
 * PC port stub — minimal declarations for d_com_inf_game.
 * Provides just enough for m_Do_main.cpp to compile.
 */
#ifndef D_COM_D_COM_INF_GAME_H
#define D_COM_D_COM_INF_GAME_H

#include "port/types.h"
#include "JSystem/JKernel/JKRHeap.h"

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

// Free functions
inline void dComIfG_dumpResControl() {}
inline void dComIfG_ct() {}
inline int  dComIfGp_roomControl_getStayNo() { return 0; }
inline u8   dComIfGp_isPauseFlag() { return 0; }

// Particle stubs — all return nullptr / no-op on PC
inline JPABaseEmitter* dComIfGp_particle_set(u16, const cXyz*, const csXyz*, const cXyz*) {
    return nullptr;
}
inline JPABaseEmitter* dComIfGp_particle_set(u16, const cXyz*, const dKy_tevstr_c*,
                                              const csXyz*, const cXyz*) {
    return nullptr;
}
inline void dComIfGp_particle_setSimple(u16, const cXyz*, u8, _GXColor&, float) {}
inline void dComIfGp_particle_setSimple(u16, const cXyz*, u8, _GXColor&, float,
                                         dPa_levelEcallBack*) {}
inline void dComIfGp_drawSimpleModel() {}
inline void dComIfGd_peekZdata() {}

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

// f_op actor manager init (defined in f_op_actor_mng.cpp / port stubs)
inline void fopAcM_initManager() {}

// Audio (defined in m_Do_audio.cpp / port stubs)
extern JKRSolidHeap* g_mDoAud_audioHeap;
inline void mDoAud_Execute() {}

#endif /* D_COM_D_COM_INF_GAME_H */
