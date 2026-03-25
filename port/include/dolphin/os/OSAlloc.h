#pragma once
/**
 * PC stub for dolphin/os/OSAlloc.h
 * Maps GC heap allocator to malloc/free on PC.
 */
#include "port/types.h"
#include <cstdlib>

#ifdef __cplusplus
extern "C" {
#endif

typedef int OSHeapHandle;

// Heap handles — we ignore them and use a global allocator
#define OS_HEAP_SYSTEM  (-1)

extern volatile OSHeapHandle __OSCurrHeap;

inline void* OSAllocFromHeap(int /*heap*/, u32 size) { return malloc(size); }
inline void  OSFreeToHeap(int /*heap*/, void* ptr)   { free(ptr); }
inline int   OSSetCurrentHeap(int heap) { int prev = __OSCurrHeap; __OSCurrHeap = heap; return prev; }

// These are no-ops on PC — memory is managed by the OS
inline void* OSInitAlloc(void* /*start*/, void* /*end*/, int /*maxHeaps*/) { return nullptr; }
inline int   OSCreateHeap(void* /*start*/, void* /*end*/) { return 0; }
inline void  OSDestroyHeap(int /*heap*/) {}
inline void  OSAddToHeap(int /*heap*/, void* /*start*/, void* /*end*/) {}
inline s32   OSCheckHeap(int /*heap*/) { return 0; }
inline u32   OSReferentSize(void* /*ptr*/) { return 0; }
inline void  OSDumpHeap(int /*heap*/) {}
inline void  OSVisitAllocated(void (* /*visitor*/)(void*, u32)) {}
inline void* OSAllocFixed(void* /*rstart*/, void* /*rend*/) { return nullptr; }

#define OSAlloc(size) OSAllocFromHeap(__OSCurrHeap, (size))
#define OSFree(ptr)   OSFreeToHeap(__OSCurrHeap, (ptr))

#ifdef __cplusplus
}
#endif
