#pragma once
/**
 * port/compat.h
 * Compiler compatibility layer.
 *
 * Provides x86 equivalents for all PowerPC / Metrowerks-specific
 * intrinsics and pragmas used in the decompiled game source.
 *
 * Include order: this file must be processed before any game source.
 * It is pulled in automatically via port/port.h.
 */

// -----------------------------------------------------------------------
// Silence Metrowerks pragmas that other compilers don't understand
// -----------------------------------------------------------------------
#if defined(__GNUC__) || defined(__clang__)
#  pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif

// Strip Metrowerks-only pragmas via macros (they're textually present in
// the source but only active when __MWERKS__ is defined).
// Nothing needed here — the game code wraps them in #ifdef __MWERKS__.

// -----------------------------------------------------------------------
// NO_INLINE — works for all compilers we target
// -----------------------------------------------------------------------
#ifndef NO_INLINE
#  if defined(_MSC_VER)
#    define NO_INLINE __declspec(noinline)
#  elif defined(__GNUC__) || defined(__clang__)
#    define NO_INLINE __attribute__((noinline))
#  else
#    define NO_INLINE
#  endif
#endif

// -----------------------------------------------------------------------
// PowerPC intrinsics → x86 equivalents
// -----------------------------------------------------------------------

// __cntlzw(x) — count leading zeros (32-bit)
// Use inline function (NOT macro) so it doesn't clash with the extern
// declaration in global.h which is compiled on non-Metrowerks builds.
#if defined(_MSC_VER)
#  include <intrin.h>
#endif
inline int __cntlzw(unsigned int x) {
#if defined(_MSC_VER)
    unsigned long idx;
    return _BitScanReverse(&idx, x) ? (31 - (int)idx) : 32;
#elif defined(__GNUC__) || defined(__clang__)
    return x ? __builtin_clz(x) : 32;
#else
    if (!x) return 32;
    int n = 0;
    if (!(x >> 16)) { n += 16; x <<= 16; }
    if (!(x >> 24)) { n +=  8; x <<=  8; }
    if (!(x >> 28)) { n +=  4; x <<=  4; }
    if (!(x >> 30)) { n +=  2; x <<=  2; }
    if (!(x >> 31))   n +=  1;
    return n;
#endif
}

// __cntlzd(x) — count leading zeros (64-bit)
inline int __cntlzd(unsigned long long x) {
#if defined(_MSC_VER)
    unsigned long idx;
    return _BitScanReverse64(&idx, x) ? (63 - (int)idx) : 64;
#elif defined(__GNUC__) || defined(__clang__)
    return x ? __builtin_clzll(x) : 64;
#else
    if (x >> 32) return __cntlzw((unsigned int)(x >> 32));
    return 32 + __cntlzw((unsigned int)x);
#endif
}

// __frsqrte / __frsqrtes — fast reciprocal square root estimate.
// Defined as inline functions in JSystem/JMath/JMath.h (port stub).
// Include it here so all TUs get them consistently as functions (not macros).
#include <cmath>
#include "JSystem/JMath/JMath.h"

// __fabs — same as fabsf/fabs in standard math
#ifndef __fabs
#  define __fabs(x) fabs(x)
#endif

// __fsel(a,b,c) — (a >= 0) ? b : c  (floating-point select)
#ifndef __fsel
#  define __fsel(a, b, c) (((a) >= 0.0) ? (b) : (c))
#endif

// __rlwimi / __rlwinm — rotate/mask word immediates
// Inline function (not macro) to co-exist with global.h's extern declaration.
inline int __rlwimi(int r, int v, int sh, int mb, int me) {
    unsigned int mask = ((0xFFFFFFFFu >> mb) & (0xFFFFFFFFu << (31 - me)));
    unsigned int rot  = ((unsigned int)v << sh) | ((unsigned int)v >> (32 - sh));
    return (int)(((unsigned int)r & ~mask) | (rot & mask));
}
inline int __rlwinm(int r, int sh, int mb, int me) {
    unsigned int mask = ((0xFFFFFFFFu >> mb) & (0xFFFFFFFFu << (31 - me)));
    unsigned int rot  = ((unsigned int)r << sh) | ((unsigned int)r >> (32 - sh));
    return (int)(rot & mask);
}

// Cache operations → no-ops on PC (inline functions to avoid macro/extern clashes)
#include <cstring>
inline void __dcbf(void* /*base*/, int /*offset*/) {}
inline void __dcbz(void* base, int offset) { memset((char*)base + offset, 0, 32); }
inline void __dcbt(void* /*base*/, int /*offset*/) {}
inline void __dcbst(void* /*base*/, int /*offset*/) {}
inline void __dcbi(void* /*base*/, int /*offset*/) {}
inline void __icbi(void* /*base*/, int /*offset*/) {}

// __sync — memory barrier
#include <atomic>
inline void __sync() { std::atomic_thread_fence(std::memory_order_seq_cst); }

// __abs — integer absolute value (inline function; avoids macro clash)
#include <cstdlib>
inline int __abs(int x) { return x < 0 ? -x : x; }

// __memcpy — standard memcpy (inline function)
inline void* __memcpy(void* d, const void* s, int n) { return memcpy(d, s, (size_t)n); }

// PPCHalt — halt the processor → abort() on PC
#ifndef PPCHalt
#  define PPCHalt() (abort())
#endif

// __PPCHalt — same
#ifndef __PPCHalt
#  define __PPCHalt() (abort())
#endif

// OSSwitchFiberEx — Metrowerks coroutine primitive; not needed on PC
#ifndef OSSwitchFiberEx
#  define OSSwitchFiberEx(a,b,c,d,e,f) ((void)0)
#endif

// __REGISTER — Metrowerks register parameter extension (no-op on standard C++)
#define __REGISTER

// ASM macro used in some files (should already be empty outside Metrowerks,
// but define it here as a safety net)
#ifndef ASM
#  define ASM
#endif

// -----------------------------------------------------------------------
// SJIS string literal helpers (used in Japanese region strings)
// -----------------------------------------------------------------------
// On PC all strings are treated as UTF-8 / Latin-1; no transformation needed.
#ifndef SJIS
#  define SJIS(s) (s)
#endif

// -----------------------------------------------------------------------
// typeof / decltype compatibility
// -----------------------------------------------------------------------
// C++11 has decltype natively; the Metrowerks __decltype__ macro is already
// handled in global.h via #ifdef __MWERKS__.

// -----------------------------------------------------------------------
// POINTER_ADD for non-GCC compilers (__typeof__ not available in MSVC)
// -----------------------------------------------------------------------
#ifdef _MSC_VER
#  undef  POINTER_ADD
#  define POINTER_ADD(ptr_, offset_) \
       reinterpret_cast<decltype(ptr_)>(reinterpret_cast<uintptr_t>(ptr_) + \
                                        static_cast<uintptr_t>(offset_))
#endif
