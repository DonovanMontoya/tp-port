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

// Cache ops are defined in OSCache.h — don't redefine here.

#ifdef __cplusplus
}
#endif
