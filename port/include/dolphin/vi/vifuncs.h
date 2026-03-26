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

static inline VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback cb) { return cb; }
static inline VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback cb) { return cb; }
static inline void VIInit(void) {}
static inline void VIWaitForRetrace(void) {}
static inline void VIConfigure(const GXRenderModeObj* /*rm*/) {}
static inline void VIConfigurePan(u16 /*xOrg*/, u16 /*yOrg*/, u16 /*width*/, u16 /*height*/) {}
static inline void VIFlush(void) {}
static inline void VISetNextFrameBuffer(void* /*fb*/) {}
static inline void VISetNextRightFrameBuffer(void* /*fb*/) {}

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
