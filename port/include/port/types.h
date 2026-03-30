#pragma once
/**
 * port/types.h
 * Re-exports the GameCube primitive types using standard <cstdint> so
 * they work correctly on x86 with MSVC/Clang/GCC.
 *
 * On PowerPC, `long` is 32-bit in both 32-bit and 64-bit modes when using
 * the EABI.  On 64-bit x86 Windows (LLP64), `long` is also 32-bit — but on
 * Linux/macOS (LP64) it is 64-bit.  We use fixed-width types to stay safe.
 *
 * This header must be included BEFORE dolphin/types.h to shadow it, or the
 * CMake include path ordering must place port/include first.
 */

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

// Primitive integer types used throughout the GC codebase
typedef int8_t    s8;
typedef uint8_t   u8;
typedef int16_t   s16;
typedef uint16_t  u16;
typedef int32_t   s32;
typedef uint32_t  u32;
typedef int64_t   s64;
typedef uint64_t  u64;

typedef volatile u8  vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8  vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef float  f32;
typedef double f64;

typedef volatile f32 vf32;
typedef volatile f64 vf64;

typedef char*        Ptr;
typedef int          BOOL;
typedef unsigned int uint;

#ifndef FALSE
#  define FALSE 0
#endif
#ifndef TRUE
#  define TRUE 1
#endif

// -----------------------------------------------------------------------
// Attribute macros — MSVC/Clang/GCC compatible versions of GC attributes
// -----------------------------------------------------------------------
#ifndef ATTRIBUTE_ALIGN
#  if defined(_MSC_VER)
#    define ATTRIBUTE_ALIGN(n) __declspec(align(n))
#  else
#    define ATTRIBUTE_ALIGN(n) __attribute__((aligned(n)))
#  endif
#endif

#ifndef DECL_WEAK
#  if defined(_MSC_VER)
     // MSVC has no direct equivalent; use __declspec(selectany) as proxy
#    define DECL_WEAK __declspec(selectany)
#  else
#    define DECL_WEAK __attribute__((weak))
#  endif
#endif

// AT_ADDRESS only makes sense on the target hardware; strip it on PC
#define AT_ADDRESS(addr)

// Metrowerks register keyword — not supported; strip it
#define register

// -----------------------------------------------------------------------
// NULL / nullptr
// -----------------------------------------------------------------------
#ifndef NULL
#  define NULL nullptr
#endif
