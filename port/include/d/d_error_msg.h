#pragma once
/**
 * port/include/d/d_error_msg.h
 * PC port stub — error message overlays (DVD errors, shutdown, etc.)
 * All execute() methods return false (no error) so fpcM_Management
 * proceeds normally on PC.
 */
#ifndef D_D_ERROR_MSG_H
#define D_D_ERROR_MSG_H

#include "port/types.h"

struct dShutdownErrorMsg_c {
    static bool execute() { return false; }
};

struct dDvdErrorMsg_c {
    static void draw(s32 /*arg*/) {}
    static bool execute() { return false; }
};

struct dConnectErrorMsg_c {
    static void disable() {}
};

#endif /* D_D_ERROR_MSG_H */
