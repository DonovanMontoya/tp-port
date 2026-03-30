/**
 * port/include/revolution/wpad.h
 * PC stub for Wii Remote (WPAD) API.
 */
#pragma once
#include "port/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WPAD_CHAN0  0
#define WPAD_CHAN1  1
#define WPAD_CHAN2  2
#define WPAD_CHAN3  3
#define WPAD_MAX_CONTROLLERS 4

#define WPAD_ERR_NONE        0
#define WPAD_ERR_NO_CONTROLLER (-1)
#define WPAD_ERR_BUSY        (-2)

#define WPAD_BUTTON_LEFT   0x0001
#define WPAD_BUTTON_RIGHT  0x0002
#define WPAD_BUTTON_DOWN   0x0004
#define WPAD_BUTTON_UP     0x0008
#define WPAD_BUTTON_PLUS   0x0010
#define WPAD_BUTTON_2      0x0100
#define WPAD_BUTTON_1      0x0200
#define WPAD_BUTTON_B      0x0400
#define WPAD_BUTTON_A      0x0800
#define WPAD_BUTTON_MINUS  0x1000
#define WPAD_BUTTON_HOME   0x8000
#define WPAD_BUTTON_Z      0x2000

typedef struct WPADStatus {
    u16 button;
    s8  accX, accY, accZ;
    u8  err;
} WPADStatus;

typedef struct WPADInfo {
    BOOL lowBat;
    u32 devType;
} WPADInfo;

typedef void (*WPADConnectCallback)(s32 chan, s32 result);
typedef void (*WPADSamplingCallback)(u32 freq);

static inline void WPADInit(void) {}
static inline s32  WPADRead(s32 /*chan*/, void* /*data*/) { return WPAD_ERR_NO_CONTROLLER; }
static inline s32  WPADProbe(s32 /*chan*/, u32* /*type*/) { return WPAD_ERR_NO_CONTROLLER; }
static inline void WPADSetConnectCallback(s32 /*chan*/, WPADConnectCallback /*cb*/) {}
static inline void WPADSetSamplingCallback(WPADSamplingCallback /*cb*/) {}
static inline s32  WPADGetStatus(void) { return 1; }
static inline void WPADShutdown(void) {}
static inline void WPADEnableURCC(BOOL /*on*/) {}
static inline void WPADEnableMotionPlus(s32 /*chan*/, u8 /*on*/) {}
static inline s32  WPADGetDataFormat(s32 /*chan*/) { return 0; }
static inline void WPADSetDataFormat(s32 /*chan*/, s32 /*fmt*/) {}
static inline BOOL WPADIsSpeakerEnabled(s32 /*chan*/) { return FALSE; }
static inline s32  WPADControlSpeaker(s32 /*chan*/, s32 /*cmd*/, void* /*cb*/) { return WPAD_ERR_NONE; }

#ifdef __cplusplus
}
#endif
