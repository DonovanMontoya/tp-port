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
#  define JUT_CONFIRM(LINE, COND) \
       do { if (!(COND)) { tp::log::error("Confirm failed: %s line %d", __FILE__, (int)(LINE)); abort(); } } while (0);
#  define JUT_ASSERT(LINE, COND) \
       do { if (!(COND)) { tp::log::error("Assert failed: %s line %d", __FILE__, (int)(LINE)); abort(); } } while (0);
#  define JUT_ASSERT_MSG(LINE, COND, MSG) \
       do { if (!(COND)) { tp::log::error("Assert failed (%s): %s line %d", (MSG), __FILE__, (int)(LINE)); abort(); } } while (0);
#  define JUT_ASSERT_MSG_F(LINE, COND, MSG, ...) \
       do { if (!(COND)) { tp::log::error("Assert failed: %s line %d", __FILE__, (int)(LINE)); abort(); } } while (0);
#  define J3D_PANIC(LINE, COND, MSG) \
       do { if (!(COND)) { tp::log::error("%s: %s line %d", (MSG), __FILE__, (int)(LINE)); abort(); } } while (0);
#  define JUT_PANIC(LINE, TEXT) \
       do { tp::log::error("%s: %s line %d", (TEXT), __FILE__, (int)(LINE)); abort(); } while (0);
#  define JUT_WARN_DEVICE(LINE, DEVICE, ...) do { (void)sizeof(DEVICE); tp::log::error(__VA_ARGS__); } while (0);
#  define JUT_WARN(LINE, ...) JUT_WARN_DEVICE((LINE), JUTAssertion::getSDevice(), __VA_ARGS__);
#  define ASSERTLINE(LINE, COND) JUT_ASSERT((LINE), (COND));
#else
#  define JUT_CONFIRM(LINE, COND) do { (void)sizeof(COND); } while (0);
#  define JUT_ASSERT(LINE, COND) do { (void)sizeof(COND); } while (0);
#  define JUT_ASSERT_MSG(LINE, COND, MSG) do { (void)sizeof(COND); (void)sizeof(MSG); } while (0);
#  define JUT_ASSERT_MSG_F(LINE, COND, MSG, ...) do { (void)sizeof(COND); (void)sizeof(MSG); } while (0);
#  define J3D_PANIC(LINE, COND, MSG) do { (void)sizeof(COND); (void)sizeof(MSG); } while (0);
#  define JUT_PANIC(LINE, TEXT) do { (void)sizeof(TEXT); } while (0);
#  define JUT_WARN_DEVICE(LINE, DEVICE, ...) do { (void)sizeof(LINE); (void)sizeof(DEVICE); } while (0);
#  define JUT_WARN(LINE, ...) do { (void)sizeof(LINE); } while (0);
#  define ASSERTLINE(LINE, COND) do { (void)sizeof(COND); } while (0);
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
    static void  setMessageCount(int /*n*/) {}
    static int   getMessageCount()          { return 0; }
    static void  setVisible(bool /*visible*/) {}
};
