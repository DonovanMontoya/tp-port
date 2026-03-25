#pragma once
/**
 * PC stub for dolphin/vi/vifuncs.h
 * Function declarations kept identical to the original.
 * Trivial query functions are implemented as inline no-ops returning 0/NULL.
 * VIInit / VIWaitForRetrace / VIConfigure / VIFlush / VISetNextFrameBuffer
 * are declared only — they are implemented in src/dolphin/vi.cpp where they
 * drive the PC window / swap-chain.
 */

#include "port/types.h"
#include "dolphin/vi/vitypes.h"
#include "dolphin/gx/GXStruct.h"

#ifdef __cplusplus
extern "C" {
#endif

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback cb);
VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback cb);
void VIInit(void);
void VIWaitForRetrace(void);
void VIConfigure(const GXRenderModeObj* rm);
void VIConfigurePan(u16 xOrg, u16 yOrg, u16 width, u16 height);
void VIFlush(void);
void VISetNextFrameBuffer(void* fb);
void VISetNextRightFrameBuffer(void* fb);

static inline void  VISetBlack(BOOL black)          { (void)black; }
static inline void  VISet3D(BOOL threeD)             { (void)threeD; }
static inline u32   VIGetRetraceCount(void)          { return 0; }
static inline u32   VIGetNextField(void)             { return 0; }
static inline u32   VIGetCurrentLine(void)           { return 0; }
static inline u32   VIGetTvFormat(void)              { return VI_NTSC; }
static inline void* VIGetNextFrameBuffer(void)       { return 0; }
static inline void* VIGetCurrentFrameBuffer(void)    { return 0; }
static inline u32   VIGetScanMode(void)              { return VI_INTERLACE; }
static inline u32   VIGetDTVStatus(void)             { return 0; }

#ifdef __cplusplus
}
#endif
