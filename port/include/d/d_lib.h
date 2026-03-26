#pragma once
/**
 * port/include/d/d_lib.h
 * PC port stub — game library helpers.
 * Shadows the original which includes STControl (heavy game logic).
 * f_pc_manager.cpp only calls dLib_time_c::startTime()/stopTime().
 */
#ifndef D_D_LIB_H
#define D_D_LIB_H

#include "port/types.h"

class fopAc_ac_c;
class cXyz;
class JKRAramArchive;

struct dLib_time_c {
    static void startTime() {}
    static void stopTime() {}
};

inline BOOL dLib_checkActorInRectangle(fopAc_ac_c* /*a*/, fopAc_ac_c* /*b*/,
                                        const cXyz* /*c*/, const cXyz* /*d*/) {
    return FALSE;
}
inline u8 dLib_getEventSwitchNo(int /*param*/) { return 0; }
inline u32 dLib_getExpandSizeFromAramArchive(JKRAramArchive* /*arc*/, const char* /*name*/) {
    return 0;
}

#endif /* D_D_LIB_H */
