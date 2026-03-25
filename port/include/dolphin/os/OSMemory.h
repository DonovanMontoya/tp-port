#pragma once
#include "port/types.h"

#ifdef __cplusplus
extern "C" {
#endif

// GC had 24 MB RAM.  On PC we don't restrict the heap.
#define OS_MEM1_SIZE (24 * 1024 * 1024)
#define OS_MEM2_SIZE (64 * 1024 * 1024)  // Wii MEM2

void* OSGetMEM1ArenaLo(void);
void* OSGetMEM1ArenaHi(void);
void  OSSetMEM1ArenaLo(void* lo);
void  OSSetMEM1ArenaHi(void* hi);
void* OSGetMEM2ArenaLo(void);
void* OSGetMEM2ArenaHi(void);
void  OSSetMEM2ArenaLo(void* lo);
void  OSSetMEM2ArenaHi(void* hi);

// Cache operations — no-ops on PC
inline void DCFlushRange(void* /*addr*/, u32 /*size*/) {}
inline void DCInvalidateRange(void* /*addr*/, u32 /*size*/) {}
inline void DCStoreRange(void* /*addr*/, u32 /*size*/) {}
inline void ICInvalidateRange(void* /*addr*/, u32 /*size*/) {}
inline void DCZeroRange(void* addr, u32 size) { memset(addr, 0, size); }

#ifdef __cplusplus
}
#endif
