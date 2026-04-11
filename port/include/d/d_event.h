#pragma once
/**
 * port/include/d/d_event.h
 * PC port stub — event control system.
 * Shadows the original which includes d/actor/d_a_itembase.h (heavy deps).
 * All event ordering is a no-op on PC.
 */
#ifndef D_EVENT_D_EVENT_H
#define D_EVENT_D_EVENT_H

#include "port/types.h"

struct dStage_MapEvent_dt_c;
class fopAc_ac_c;

#if TP_PORT_USE_REAL_ALINK
#include "../../../include/d/actor/d_a_itembase.h"
#endif

enum dEvt_mode_e {
    dEvt_mode_WAIT_e       = 0,
    dEvt_mode_TALK_e       = 1,
    dEvt_mode_DEMO_e       = 2,
    dEvt_mode_COMPULSORY_e = 3,
};

enum dEvt_type_e {
    dEvt_type_TALK_e       = 0,
    dEvt_type_DOOR_e       = 1,
    dEvt_type_OTHER_e      = 2,
    dEvt_type_COMPULSORY_e = 3,
    dEvt_type_POTENTIAL_e  = 4,
    dEvt_type_ITEM_e       = 5,
    dEvt_type_SHOWITEM_X_e = 6,
    dEvt_type_SHOWITEM_Y_e = 7,
    dEvt_type_CATCH_e      = 10,
    dEvt_type_TREASURE_e   = 11,
};

// Minimal event control stub — no event runs on PC
class dEvt_control_c {
public:
    dEvt_control_c() {}

    // Order never succeeds
    s32 order(u16 /*type*/, u16 /*prio*/, u16 /*flag*/, u16 /*hind*/,
              void* /*req*/, void* /*tgt*/, s16 /*evId*/, u8 /*toolId*/) { return -1; }

    // Ordering is always "OK" so callers can proceed, but order() returns -1
    BOOL isOrderOK()         { return TRUE; }
    BOOL isChangeOK(void*)   { return FALSE; }
    BOOL runCheck()          { return FALSE; }

    // Partner accessors
    fopAc_ac_c* getPt1()     { return nullptr; }
    fopAc_ac_c* getPt2()     { return nullptr; }
    fopAc_ac_c* getPtT()     { return nullptr; }
    fopAc_ac_c* getPtI()     { return nullptr; }
    fopAc_ac_c* getPtD()     { return nullptr; }
    void setPtT(void*) {}
    void setPtI(void*) {}
    void setPtI_Id(fpc_ProcID) {}
    void setPtD(void*) {}

    // Cull rate
    f32  getCullRate()        { return 1.0f; }
    void setCullRate(f32)     {}

    // Item getter
    u8   getGtItm()           { return 0; }
    void setGtItm(u8)         {}
    u8   getPreItemNo()       { return 0xFF; }

    // Misc
    u8  getMode()             { return dEvt_mode_WAIT_e; }
    u8  getMapToolId()        { return 0xFF; }
    u16 chkEventFlag(u16)     { return 0; }
    void onEventFlag(u16)     {}
    void offEventFlag(u16)    {}
    u16 checkHind(u16)        { return 0; }
    void onHindFlag(u16)      {}
    void offHindFlag(u16)     {}
    BOOL chkTalkXY()          { return FALSE; }
    u8  checkCompulsory()     { return 0; }
    bool checkSkipEdge()      { return false; }
    void startCheckSkipEdge(void*) {}
    void remove()             {}
    void reset()              {}
    void reset(void*)         {}
    int  moveApproval(void*)  { return 2; }  // 2 = free move
    BOOL compulsory(void*, const char*, u16) { return FALSE; }
    int  Step()               { return 0; }

    dStage_MapEvent_dt_c* getStageEventDt() { return nullptr; }

    static dStage_MapEvent_dt_c* searchMapEventData(u8, s32) { return nullptr; }
    static dStage_MapEvent_dt_c* searchMapEventData(u8)      { return nullptr; }
};

#endif /* D_EVENT_D_EVENT_H */
