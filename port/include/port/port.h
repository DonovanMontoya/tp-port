#pragma once
/**
 * port/port.h
 * Top-level include for the PC port layer.
 *
 * Include this file (directly or via precompiled header) before any
 * GameCube SDK or game headers so that platform guards are set correctly.
 */

// -----------------------------------------------------------------------
// Platform guard — must be defined before any dolphin/ headers are pulled in.
// CMakeLists.txt also passes -DPLATFORM_PC=1 on the command line; this
// fallback covers direct includes from IDE tooling.
// -----------------------------------------------------------------------
#ifndef PLATFORM_PC
#  define PLATFORM_PC 1
#endif
#ifndef PLATFORM_GC
#  define PLATFORM_GC 0
#endif
#ifndef PLATFORM_WII
#  define PLATFORM_WII 0
#endif
#ifndef PLATFORM_SHIELD
#  define PLATFORM_SHIELD 0
#endif

// -----------------------------------------------------------------------
// Standard C++ headers always available on the host
// -----------------------------------------------------------------------
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cassert>

// -----------------------------------------------------------------------
// Port sub-headers
// -----------------------------------------------------------------------
#include "port/endian.h"
#include "port/types.h"
#include "port/logging.h"
#include "port/compat.h"
// Pull in the port's dolphin/os.h early so its include guard (_DOLPHIN_OS_H_)
// blocks the original libs/dolphin version from being included later and
// redefining OS_REPORT to nothing.
#include "dolphin/os.h"
// Pull in gx.h early so GXGamma/GXRenderModeObj/etc. are defined before
// any game header that forward-declares them with incompatible types.
#include "dolphin/gx.h"

