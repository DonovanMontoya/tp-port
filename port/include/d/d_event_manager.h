#pragma once
/**
 * port/include/d/d_event_manager.h
 * PC port stub — event manager class.
 * All event lookups return "not found" on PC.
 */
#ifndef D_EVENT_MANAGER_D_EVENT_MANAGER_H
#define D_EVENT_MANAGER_D_EVENT_MANAGER_H

#include "port/types.h"
#include "d/d_event_data.h"
#include "SSystem/SComponent/c_xyz.h"

class fopAc_ac_c;

class dEvent_manager_c {
public:
    dEvent_manager_c()  {}
    ~dEvent_manager_c() {}

    void create() {}
    void remove() {}
    void* getEventData(s16) { return nullptr; }
    s16 getEventIdx(const char* /*name*/, u8 /*toolId*/, s32 /*roomNo*/) { return -1; }
    s16 getEventIdx(fopAc_ac_c* /*actor*/, u8 /*toolId*/)                { return -1; }
    s16 getEventIdx(fopAc_ac_c* /*actor*/, const char* /*name*/, u8 /*toolId*/) { return -1; }
    u16 getEventPrio(fopAc_ac_c* /*actor*/, s16 /*evId*/)               { return 0; }
    BOOL isMapToolCamera(u8, s32) { return FALSE; }
    BOOL startCheck(s16) { return FALSE; }
    BOOL startCheckOld(const char*) { return FALSE; }
    BOOL endCheck(s16) { return FALSE; }
    BOOL endCheckOld(const char*) { return FALSE; }
    int getMyStaffId(const char*, fopAc_ac_c*, int) { return -1; }
    int getIsAddvance(int) { return FALSE; }
    int getMyActIdx(int, char**, int, BOOL, BOOL) { return -1; }
    void* getMySubstanceP(int, char*, int) { return nullptr; }
    int getMySubstanceNum(int, char*) { return 0; }
    char* getMyNowCutName(int) { return nullptr; }
    const char* getRunEventName() { return ""; }
    void cutEnd(int) {}
    void setGoal(cXyz*) {}
    cXyz* getGoal() { return nullptr; }
    BOOL ChkPresentEnd() { return TRUE; }
    int checkStartDemo() { return 0; }
    void orderStartDemo() {}
    bool dataLoaded() { return true; }
    void demoInit() {}
    void roomInit(int) {}
    void roomFinish(int) {}

    void setObjectArchive(const char*) {}
    int  cameraPlay()                  { return 0; }
    void setCameraPlay(int)            {}
    int  setStartDemo(int)             { return 0; }
};

#endif /* D_EVENT_MANAGER_D_EVENT_MANAGER_H */
