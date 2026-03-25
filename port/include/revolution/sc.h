/**
 * port/include/revolution/sc.h
 * PC stub for Wii System Configuration (SC) API.
 * All SC functions are no-ops / return safe defaults.
 * This header is only included on PLATFORM_WII builds; on GCN it's guarded.
 */
#pragma once
#include "port/types.h"

#ifdef __cplusplus
extern "C" {
#endif

// SC aspect ratio / progressive scan (return GCN-safe defaults)
typedef enum {
    SC_ASPECT_RATIO_4x3  = 0,
    SC_ASPECT_RATIO_16x9 = 1,
} SCDisplayAspectRatio;

typedef enum {
    SC_PROGRESSIVE_MODE_OFF = 0,
    SC_PROGRESSIVE_MODE_ON  = 1,
} SCProgressiveMode;

typedef enum {
    SC_SOUND_MONO   = 0,
    SC_SOUND_STEREO = 1,
    SC_SOUND_SURROUND = 2,
} SCSoundMode;

typedef void (*SCFlushCallback)(s32 result);

static inline void SCInit(void) {}
static inline s32  SCGetAspectRatio(void) { return SC_ASPECT_RATIO_4x3; }
static inline s32  SCGetProgressiveMode(void) { return SC_PROGRESSIVE_MODE_OFF; }
static inline s32  SCGetSoundMode(void) { return SC_SOUND_STEREO; }
static inline s32  SCGetScreenPositionX(void) { return 0; }
static inline s32  SCGetScreenPositionY(void) { return 0; }
static inline s32  SCGetScreenSizeX(void) { return 0; }
static inline s32  SCGetScreenSizeY(void) { return 0; }
static inline BOOL SCSetAspectRatio(s32 /*ratio*/) { return TRUE; }
static inline BOOL SCSetProgressiveMode(s32 /*mode*/) { return TRUE; }
static inline BOOL SCSetSoundMode(s32 /*mode*/) { return TRUE; }
static inline BOOL SCFlush(SCFlushCallback /*cb*/) { return TRUE; }
static inline s32  SCGetStatus(void) { return 1; } // SC_STATUS_READY
static inline s32  SCCheckStatus(void) { return 0; }
static inline BOOL SCGetLanguage(u8* /*lang*/) { return FALSE; }
static inline BOOL SCGetArea(u8* /*area*/) { return FALSE; }
static inline BOOL SCGetCounterBias(u32* /*bias*/) { return FALSE; }
static inline BOOL SCGetSimpleModel(u8* /*model*/) { return FALSE; }
static inline BOOL SCGetWiiConnect24Enabled(u8* /*en*/) { return FALSE; }
static inline BOOL SCSetScreenPosition(u32 /*x*/, u32 /*y*/) { return TRUE; }
static inline BOOL SCGetDvdDelay(u8* /*delay*/) { return FALSE; }

#ifdef __cplusplus
}
#endif
