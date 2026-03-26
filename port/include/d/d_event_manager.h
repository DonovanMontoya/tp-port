#pragma once
/**
 * port/include/d/d_event_manager.h
 * PC port stub — event manager class.
 * All event lookups return "not found" on PC.
 */
#ifndef D_EVENT_MANAGER_D_EVENT_MANAGER_H
#define D_EVENT_MANAGER_D_EVENT_MANAGER_H

#include "port/types.h"

class fopAc_ac_c;

class dEvent_manager_c {
public:
    dEvent_manager_c()  {}
    ~dEvent_manager_c() {}

    s16 getEventIdx(const char* /*name*/, u8 /*toolId*/, s32 /*roomNo*/) { return -1; }
    s16 getEventIdx(fopAc_ac_c* /*actor*/, u8 /*toolId*/)                { return -1; }
    s16 getEventIdx(fopAc_ac_c* /*actor*/, const char* /*name*/, u8 /*toolId*/) { return -1; }
    u16 getEventPrio(fopAc_ac_c* /*actor*/, s16 /*evId*/)               { return 0; }

    void setObjectArchive(const char*) {}
    int  cameraPlay()                  { return 0; }
    int  setStartDemo(int)             { return 0; }
};

#endif /* D_EVENT_MANAGER_D_EVENT_MANAGER_H */
