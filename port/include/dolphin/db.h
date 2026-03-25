#pragma once
/**
 * PC stub for dolphin/db.h
 * The GC debugger interface (IS-DOL BBA + DBInterface register at 0x40) is
 * not present on PC.  DBIsDebuggerPresent always returns FALSE and all other
 * functions are no-ops.  DBPrintf is forwarded to printf so that any
 * debug-print calls are not silently lost.
 */

#include "port/types.h"
#include <cstdio>   // printf
#include <cstdarg>  // va_list

// DBInterface address constant kept for source compatibility; not used on PC.
#define OS_DBINTERFACE_ADDR 0x00000040

#ifdef __cplusplus
extern "C" {
#endif

static inline BOOL DBIsDebuggerPresent(void) { return FALSE; }
static inline s32  DBQueryData(void)         { return 0; }
static inline u32  DBRead(u8* buf, u32 len)  { (void)buf; (void)len; return 0; }

static inline void DBPrintf(char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

#ifdef __cplusplus
}
#endif
