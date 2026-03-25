#pragma once
#include "port/types.h"
#include "port/logging.h"

#ifdef __cplusplus
extern "C" {
#endif

// Panic / error macros used throughout the game
#define OSError(...)       OSPanic(__FILE__, __LINE__, __VA_ARGS__)
#define OSErrorLine(ln,...)  OSPanic(__FILE__, (ln), __VA_ARGS__)

inline void OSHalt(const char* file, int line, const char* msg) {
    tp::log::error("OSHalt %s:%d: %s", file, line, msg);
    abort();
}

#ifdef __cplusplus
}
#endif
