#pragma once
/**
 * PC stub for dolphin/sdk_math.h
 * The original used Metrowerks / PPC intrinsics (__frsqrte, __fabsf) and
 * manual Newton-Raphson sqrt refinement loops.  On PC we redirect every
 * function to the equivalent <cmath> implementation so the compiler can
 * use its own optimised intrinsics (SSE sqrtss/sqrtsd etc.).
 *
 * The original header also redefined sqrt/floor/cos/fabs as inline
 * functions, which can conflict with <cmath> declarations.  We use the
 * standard library versions throughout.
 */

#include <cmath>

// Bring standard names into scope without polluting the global namespace
// beyond what the original header provided.
using std::sqrtf;
using std::sqrt;
using std::fabsf;
using std::fabs;
using std::cosf;
using std::cos;
using std::sinf;
using std::sin;
using std::tanf;
using std::tan;
using std::acosf;
using std::acos;
using std::floorf;
using std::floor;
