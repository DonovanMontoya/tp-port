#pragma once
// Cache operations are no-ops on PC — coherent cache
#include "port/types.h"
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif

inline void DCFlushRange(void* /*addr*/, u32 /*size*/) {}
inline void DCInvalidateRange(void* /*addr*/, u32 /*size*/) {}
inline void DCStoreRange(void* /*addr*/, u32 /*size*/) {}
inline void DCStoreRangeNoSync(void* /*addr*/, u32 /*size*/) {}
inline void ICInvalidateRange(void* /*addr*/, u32 /*size*/) {}
inline void DCZeroRange(void* addr, u32 size) { memset(addr, 0, size); }
inline void DCTouchRange(void* /*addr*/, u32 /*size*/) {}
inline void LCDisable(void) {}
inline void LCEnable(void) {}

#ifdef __cplusplus
}
#endif
