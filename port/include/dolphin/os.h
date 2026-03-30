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
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

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
#include "dolphin/os/OSModule.h"

#ifdef __cplusplus
extern "C" {
#endif

// OS initialisation
void OSInit(void);

// Fundamental report functions (implemented in src/dolphin/os.cpp)
void OSReport(const char* fmt, ...);
void OSReport_Error(const char* fmt, ...);
void OSReport_Warning(const char* fmt, ...);
void OSReport_FatalError(const char* fmt, ...);
void OSReport_System(const char* fmt, ...);
void OSReportDisable(void);
void OSReportEnable(void);
void OSPanic(const char* file, int line, const char* fmt, ...);
void OSAttention(const char* fmt, ...);

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

// Boot info (GC boot block — not used on PC, provided for source compat)
#include "dolphin/dvd.h"

typedef struct OSBootInfo_s {
    DVDDiskID diskID;
    u32 magic;
    u32 version;
    u32 memorySize;
    u32 consoleType;
    void* arenaLo;
    void* arenaHi;
    void* FSTLocation;
    u32 FSTMaxLength;
} OSBootInfo;

// Arena alloc helpers (stubs — PC has no GC memory arena)
static inline void* OSAllocFromArenaLo(u32 size, u32 /*align*/) { return ::malloc(size); }
static inline void* OSAllocFromArenaHi(u32 size, u32 /*align*/) { return ::malloc(size); }
static inline void* OSAllocFromMEM1ArenaLo(u32 size, u32 align) { return OSAllocFromArenaLo(size, align); }
static inline void* OSAllocFromMEM2ArenaLo(u32 size, u32 align) { return OSAllocFromArenaLo(size, align); }

// Report initialiser
void OSReportInit(void);

// Reset code — always return 0 (cold boot, no warm reset) on PC
static inline u32  OSGetResetCode(void) { return 0; }
static inline void OSSetSaveRegion(void*, u8*) {}
#define OS_SOUND_MODE_MONO 0
#define OS_SOUND_MODE_STEREO 1
static inline u32  OSGetSoundMode(void) { return OS_SOUND_MODE_STEREO; }

// Console type — PC always reports retail console
#define OS_CONSOLE_DEVELOPMENT 0x10000000u
#define OS_CONSOLE_RETAIL      0x00000000u
static inline u32  OSGetConsoleType(void) { return OS_CONSOLE_RETAIL; }

// OS_REPORT convenience macros (always enabled on PC so debug output works)
#define OS_REPORT(...)       OSReport(__VA_ARGS__)
#define OS_REPORT_ERROR(...) OSReport_Error(__VA_ARGS__)
// OS_WARNING — debug assertion/warning (no-op in release, log on PC)
#define OS_WARNING(...)      OSReport(__VA_ARGS__)

// Progressive-mode stubs (GC/Wii only — no-op on PC)
#define OS_PROGRESSIVE_MODE_OFF 0
#define OS_PROGRESSIVE_MODE_ON  1
static inline void OSSetProgressiveMode(int /*mode*/) {}
static inline int  OSGetProgressiveMode(void)          { return OS_PROGRESSIVE_MODE_OFF; }

// Timer clock (GCN: 486MHz/4 = ~162MHz; use a reasonable constant on PC)
#ifndef OS_TIMER_CLOCK
#define OS_TIMER_CLOCK 162000000
#endif
#ifndef OS_CORE_CLOCK
#define OS_CORE_CLOCK  486000000
#endif

static inline long long OSTicksToMilliseconds(OSTime ticks) {
    return static_cast<long long>((ticks * 1000) / OS_TIMER_CLOCK);
}
