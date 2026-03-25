#pragma once
/**
 * PC stub for dolphin/pad.h
 * Maps GameCube controller API to SDL2 gamepad.
 * Implementation: src/dolphin/pad.cpp
 */
#include "port/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PAD_MAX_CONTROLLERS 4

// Button bitmask (same values as GC hardware — game code uses these directly)
#define PAD_BUTTON_LEFT   (1 << 0)
#define PAD_BUTTON_RIGHT  (1 << 1)
#define PAD_BUTTON_DOWN   (1 << 2)
#define PAD_BUTTON_UP     (1 << 3)
#define PAD_TRIGGER_Z     (1 << 4)
#define PAD_TRIGGER_R     (1 << 5)
#define PAD_TRIGGER_L     (1 << 6)
#define PAD_BUTTON_A      (1 << 8)
#define PAD_BUTTON_B      (1 << 9)
#define PAD_BUTTON_X      (1 << 10)
#define PAD_BUTTON_Y      (1 << 11)
#define PAD_BUTTON_START  (1 << 12)
#define PAD_BUTTON_MENU   PAD_BUTTON_START

#define PAD_CHAN0_BIT 0x80000000u
#define PAD_CHAN1_BIT 0x40000000u
#define PAD_CHAN2_BIT 0x20000000u
#define PAD_CHAN3_BIT 0x10000000u

#define PAD_MOTOR_STOP      0
#define PAD_MOTOR_RUMBLE    1
#define PAD_MOTOR_STOP_HARD 2

#define PAD_ERR_NONE           0
#define PAD_ERR_NO_CONTROLLER -1
#define PAD_ERR_NOT_READY     -2
#define PAD_ERR_TRANSFER      -3

#define PAD_SPEC_0 0
#define PAD_SPEC_5 5

typedef struct PADStatus {
    u16 button;
    s8  stickX;
    s8  stickY;
    s8  substickX;
    s8  substickY;
    u8  triggerLeft;
    u8  triggerRight;
    u8  analogA;
    u8  analogB;
    s8  err;
} PADStatus;

typedef struct PADClampRegion {
    u8 minTrigger, maxTrigger;
    s8 minStick, maxStick, xyStick;
    s8 minSubstick, maxSubstick, xySubstick;
    s8 radStick, radSubstick;
} PADClampRegion;

typedef void (*PADSamplingCallback)(void);

BOOL PADInit(void);
int  PADReset(u32 mask);
BOOL PADRecalibrate(u32 mask);
void PADRead(PADStatus* status);
u32  PADButtonDown(u32 last, u32 current);
u32  PADButtonUp(u32 last, u32 current);
void PADSetSpec(u32 spec);
void PADClamp(PADStatus* status);
void PADClampCircle(PADStatus* status);
void PADSetClampRegion(const PADClampRegion* clamp, u32 spec);
u32  PADGetType(u32 chan);
BOOL PADSync(void);
void PADSetSamplingCallback(PADSamplingCallback cb);
void PADControlMotor(u32 chan, u32 cmd);
void PADStopMotor(u32 chan);

#define RES_WIRELESS_LITE 0x40000

#ifdef __cplusplus
}
#endif
