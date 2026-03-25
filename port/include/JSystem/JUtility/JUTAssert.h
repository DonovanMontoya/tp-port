/**
 * port/include/JSystem/JUtility/JUTAssert.h
 * PC port stub — maps JUT assertions to standard assert/abort.
 */
#pragma once
#include "port/logging.h"
#include <cassert>
#include <cstdlib>

// Debug assertion macros (disabled in release; OSPanic handled in logging.h)
#if DEBUG
#  define JUT_ASSERT(LINE, COND) \
       ((COND) ? (void)0 : (tp::log::error("Assert failed: %s line %d", __FILE__, (int)(LINE)), abort()))
#  define JUT_ASSERT_MSG(LINE, COND, MSG) \
       ((COND) ? (void)0 : (tp::log::error("Assert failed (%s): %s line %d", (MSG), __FILE__, (int)(LINE)), abort()))
#  define JUT_ASSERT_MSG_F(LINE, COND, MSG, ...) \
       ((COND) ? (void)0 : (tp::log::error("Assert failed: %s line %d", __FILE__, (int)(LINE)), abort()))
#  define J3D_PANIC(LINE, COND, MSG) \
       ((COND) != 0 ? (void)0 : (tp::log::error("%s: %s line %d", (MSG), __FILE__, (int)(LINE)), abort()))
#  define JUT_PANIC(LINE, TEXT) \
       (tp::log::error("%s: %s line %d", (TEXT), __FILE__, (int)(LINE)), abort())
#else
#  define JUT_ASSERT(LINE, COND)                  ((void)0)
#  define JUT_ASSERT_MSG(LINE, COND, MSG)          ((void)0)
#  define JUT_ASSERT_MSG_F(LINE, COND, MSG, ...)   ((void)0)
#  define J3D_PANIC(LINE, COND, MSG)               ((void)0)
#  define JUT_PANIC(LINE, TEXT)                    ((void)0)
#endif

// Minimal JUTAssertion class stub (used by J3D etc.)
class JUTAssertion {
public:
    static void showAssert(void* /*dev*/, const char* file, int line, const char* msg) {
        tp::log::error("JUT Assert: %s  [%s:%d]", msg ? msg : "", file, line);
    }
    static void showAssert_f(void* /*dev*/, const char* file, int line, const char* fmt, ...) {
        (void)file; (void)line; (void)fmt;
    }
    static void* getSDevice() { return nullptr; }
};
