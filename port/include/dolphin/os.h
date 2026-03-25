#pragma once
// Define the original header guard so libs/dolphin/include/dolphin/os.h
// is blocked from being included (it would redefine OS_REPORT to nothing
// in non-DEBUG builds, overwriting our always-on versions).
#ifndef _DOLPHIN_OS_H_
#define _DOLPHIN_OS_H_
#endif
/**
 * PC stub for dolphin/os.h
 * Aggregates all OS sub-headers just like the original.
 */
#include "port/types.h"
#include "port/logging.h"

#include "dolphin/os/OSContext.h"
#include "dolphin/os/OSAlloc.h"
#include "dolphin/os/OSCache.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSThread.h"
#include "dolphin/os/OSMutex.h"
#include "dolphin/os/OSMessage.h"
#include "dolphin/os/OSMemory.h"
#include "dolphin/os/OSError.h"
#include "dolphin/os/OSReset.h"
#include "dolphin/os/OSTime.h"
#include "dolphin/os/OSAlarm.h"

#ifdef __cplusplus
extern "C" {
#endif

// OS initialisation
void OSInit(void);

// Fundamental report functions (implemented in src/dolphin/os.cpp)
void OSReport(const char* fmt, ...);
void OSReport_Error(const char* fmt, ...);
void OSReport_Warning(const char* fmt, ...);
void OSPanic(const char* file, int line, const char* fmt, ...);

// Arena helpers (implemented in src/dolphin/os.cpp)
void* OSGetArenaLo(void);
void* OSGetArenaHi(void);
void  OSSetArenaLo(void* lo);
void  OSSetArenaHi(void* hi);

// Physical ↔ effective address translations (trivial identity on PC)
inline void* OSPhysicalToUncached(u32 pa) { return reinterpret_cast<void*>(static_cast<uintptr_t>(pa)); }
inline void* OSPhysicalToCached(u32 pa)   { return reinterpret_cast<void*>(static_cast<uintptr_t>(pa)); }
inline u32   OSCachedToPhysical(void* ea) { return static_cast<u32>(reinterpret_cast<uintptr_t>(ea)); }

// Halt / fatal
void OSFatal(u32 fg, u32 bg, const char* msg);

// Font support (stub — no ROM font on PC)
typedef struct OSFontHeader {
    u16 fontType;
    u16 firstChar;
    u16 lastChar;
    u16 invalidChar;
    u16 ascent;
    u16 descent;
    u16 width;
    u16 leading;
    u16 cellWidth;
    u16 cellHeight;
    u32 sheetSize;
    u16 sheetFormat;
    u16 sheetColumn;
    u16 sheetRow;
    u16 sheetWidth;
    u16 sheetHeight;
    u16 widthTableOffset;
    u32 sheetImage;
    u32 sheetMetric;
} OSFontHeader;
static inline BOOL OSInitFont(OSFontHeader* /*f*/) { return FALSE; }
static inline u32  OSLoadFont(OSFontHeader* /*f*/, void* /*buf*/) { return 0; }

#ifdef __cplusplus
}
#endif

// OS_REPORT convenience macros (always enabled on PC so debug output works)
#define OS_REPORT(...)       OSReport(__VA_ARGS__)
#define OS_REPORT_ERROR(...) OSReport_Error(__VA_ARGS__)
