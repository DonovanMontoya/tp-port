#pragma once
// Interrupt masking — no-ops on PC (we're in userspace)
#include "port/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef u32 OSInterruptMask;
typedef s32 __OSInterrupt;
struct OSContext;
typedef void (*__OSInterruptHandler)(__OSInterrupt interrupt, OSContext* context);

inline u32  OSDisableInterrupts(void)       { return 0; }
inline u32  OSEnableInterrupts(void)        { return 0; }
inline u32  OSRestoreInterrupts(u32 /*lvl*/) { return 0; }
inline BOOL OSGetEXILock(u32 /*dev*/, u32 /*id*/, void* /*cb*/) { return TRUE; }

#ifdef __cplusplus
}
#endif
