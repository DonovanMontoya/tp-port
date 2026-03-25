#pragma once
#include "port/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*OSResetCallback)(void);

void OSRegisterResetFunction(OSResetCallback cb);
void OSUnregisterResetFunction(OSResetCallback cb);
BOOL OSGetResetButtonState(void);
BOOL OSGetResetSwitchState(void);
void OSResetSystem(s32 reset, u32 renderFrame, BOOL bootDol);

// Stubs
inline BOOL OSGetResetButtonState(void)  { return FALSE; }
inline BOOL OSGetResetSwitchState(void)  { return FALSE; }

#ifdef __cplusplus
}
#endif
