/**
 * src/dolphin/pad.cpp
 * PAD → SDL2 controller implementation.
 *
 * Maps SDL2 GameController API to the GC PAD layout.
 * GC controller → SDL2 mapping:
 *   A       → SDL_CONTROLLER_BUTTON_A
 *   B       → SDL_CONTROLLER_BUTTON_B
 *   X       → SDL_CONTROLLER_BUTTON_X
 *   Y       → SDL_CONTROLLER_BUTTON_Y
 *   Start   → SDL_CONTROLLER_BUTTON_START
 *   Z       → SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
 *   L       → SDL_CONTROLLER_AXIS_TRIGGERLEFT  (>50%)
 *   R       → SDL_CONTROLLER_AXIS_TRIGGERRIGHT (>50%)
 *   D-pad   → SDL_CONTROLLER_BUTTON_DPAD_*
 *   Main stick  → SDL_CONTROLLER_AXIS_LEFT*
 *   C-stick     → SDL_CONTROLLER_AXIS_RIGHT*
 *
 * Keyboard fallback (no controller present):
 *   Arrow keys → D-pad
 *   WASD       → Main stick (digital)
 *   Space      → A, X → B, Z → Start
 */

#include "port/port.h"
#include "dolphin/pad.h"

#include <SDL2/SDL.h>
#include <cstring>
#include <cmath>

static SDL_GameController* sControllers[PAD_MAX_CONTROLLERS] = {};
static SDL_Haptic*         sHaptic[PAD_MAX_CONTROLLERS]      = {};
static PADSamplingCallback sSamplingCB = nullptr;

// -----------------------------------------------------------------------
// Keyboard state for fallback
// -----------------------------------------------------------------------
static const uint8_t* sKeys = nullptr;

// -----------------------------------------------------------------------
// Axis dead-zone / normalise to GC range [-128, 127]
// -----------------------------------------------------------------------
static s8 NormaliseAxis(int16_t raw, int16_t deadzone = 4096) {
    if (raw > -deadzone && raw < deadzone) return 0;
    return (s8)(raw * 127 / 32767);
}

static u8 NormaliseTrigger(int16_t raw) {
    if (raw < 0) raw = 0;
    return (u8)(raw * 255 / 32767);
}

// -----------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------
BOOL PADInit(void) {
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) != 0) {
        tp::log::warn("PAD: SDL_Init failed: %s", SDL_GetError());
    }
    sKeys = SDL_GetKeyboardState(nullptr);

    for (int i = 0; i < PAD_MAX_CONTROLLERS; i++) {
        if (SDL_IsGameController(i)) {
            sControllers[i] = SDL_GameControllerOpen(i);
            if (sControllers[i]) {
                SDL_Joystick* joy = SDL_GameControllerGetJoystick(sControllers[i]);
                if (SDL_JoystickIsHaptic(joy)) {
                    sHaptic[i] = SDL_HapticOpenFromJoystick(joy);
                    SDL_HapticRumbleInit(sHaptic[i]);
                }
                tp::log::info("PAD: controller %d: %s", i,
                              SDL_GameControllerName(sControllers[i]));
            }
        }
    }
    return TRUE;
}

int PADReset(u32 /*mask*/) { return 1; }
BOOL PADRecalibrate(u32 /*mask*/) { return TRUE; }
BOOL PADSync(void) { SDL_GameControllerUpdate(); return TRUE; }
void PADSetSpec(u32 /*spec*/) {}
void PADSetSamplingCallback(PADSamplingCallback cb) { sSamplingCB = cb; }

void PADRead(PADStatus* status) {
    if (!status) return;
    SDL_GameControllerUpdate();
    if (sSamplingCB) sSamplingCB();

    for (int i = 0; i < PAD_MAX_CONTROLLERS; i++) {
        PADStatus& s = status[i];
        memset(&s, 0, sizeof(s));

        SDL_GameController* gc = sControllers[i];
        if (!gc && i != 0) {
            s.err = PAD_ERR_NO_CONTROLLER;
            continue;
        }

        s.err = PAD_ERR_NONE;

        if (gc) {
            // Face buttons
            if (SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_A))
                s.button |= PAD_BUTTON_A;
            if (SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_B))
                s.button |= PAD_BUTTON_B;
            if (SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_X))
                s.button |= PAD_BUTTON_X;
            if (SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_Y))
                s.button |= PAD_BUTTON_Y;
            if (SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_START))
                s.button |= PAD_BUTTON_START;
            if (SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
                s.button |= PAD_TRIGGER_Z;

            // D-pad
            if (SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
                s.button |= PAD_BUTTON_LEFT;
            if (SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
                s.button |= PAD_BUTTON_RIGHT;
            if (SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
                s.button |= PAD_BUTTON_DOWN;
            if (SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_DPAD_UP))
                s.button |= PAD_BUTTON_UP;

            // Analog triggers
            int16_t lt = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
            int16_t rt = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
            s.triggerLeft  = NormaliseTrigger(lt);
            s.triggerRight = NormaliseTrigger(rt);
            if (s.triggerLeft  > 127) s.button |= PAD_TRIGGER_L;
            if (s.triggerRight > 127) s.button |= PAD_TRIGGER_R;

            // Main stick
            s.stickX    = NormaliseAxis(SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTX));
            s.stickY    = NormaliseAxis(-SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTY));
            // C-stick
            s.substickX = NormaliseAxis(SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_RIGHTX));
            s.substickY = NormaliseAxis(-SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_RIGHTY));
        }

        // Keyboard fallback (player 0 only)
        if (i == 0 && sKeys) {
            if (sKeys[SDL_SCANCODE_SPACE])  s.button |= PAD_BUTTON_A;
            if (sKeys[SDL_SCANCODE_X])      s.button |= PAD_BUTTON_B;
            if (sKeys[SDL_SCANCODE_Z])      s.button |= PAD_BUTTON_X;
            if (sKeys[SDL_SCANCODE_C])      s.button |= PAD_BUTTON_Y;
            if (sKeys[SDL_SCANCODE_RETURN]) s.button |= PAD_BUTTON_START;
            if (sKeys[SDL_SCANCODE_LEFT])   s.button |= PAD_BUTTON_LEFT;
            if (sKeys[SDL_SCANCODE_RIGHT])  s.button |= PAD_BUTTON_RIGHT;
            if (sKeys[SDL_SCANCODE_DOWN])   s.button |= PAD_BUTTON_DOWN;
            if (sKeys[SDL_SCANCODE_UP])     s.button |= PAD_BUTTON_UP;
            // WASD → main stick
            if (sKeys[SDL_SCANCODE_A])    s.stickX = -96;
            if (sKeys[SDL_SCANCODE_D])    s.stickX =  96;
            if (sKeys[SDL_SCANCODE_W])    s.stickY =  96;
            if (sKeys[SDL_SCANCODE_S])    s.stickY = -96;
        }
    }
}

u32 PADButtonDown(u32 last, u32 current) { return (~last) & current; }
u32 PADButtonUp  (u32 last, u32 current) { return last & (~current); }

void PADControlMotor(u32 chan, u32 cmd) {
    if (chan >= PAD_MAX_CONTROLLERS || !sHaptic[chan]) return;
    if (cmd == PAD_MOTOR_RUMBLE)
        SDL_HapticRumblePlay(sHaptic[chan], 0.8f, 200);
    else
        SDL_HapticRumbleStop(sHaptic[chan]);
}
void PADStopMotor(u32 chan) { PADControlMotor(chan, PAD_MOTOR_STOP); }

void PADClamp(PADStatus* /*s*/)       {}
void PADClampCircle(PADStatus* /*s*/) {}
void PADSetClampRegion(const PADClampRegion* /*r*/, u32 /*spec*/) {}
u32  PADGetType(u32 /*chan*/)         { return 0; }
