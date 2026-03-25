#pragma once
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

#ifdef __cplusplus
}
#endif
