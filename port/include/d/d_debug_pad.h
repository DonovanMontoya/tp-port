#pragma once
/**
 * port/include/d/d_debug_pad.h
 * PC port stub — dDebugPad_c debug pad input handler.
 */
#ifndef D_DEBUG_PAD_H
#define D_DEBUG_PAD_H

class dDebugPad_c {
public:
    enum Mode_e {
        MODE_CAMERA_e,
        MODE_LIGHT_e,
        MODE_WINDTEST_e,
        MODE_SDCARD_e,
        MODE_MAX_e,
    };
    dDebugPad_c() : mIsActive(false) {}
    bool Active() { return mIsActive; }
    bool Update() { return false; }
    bool mIsActive;
};

extern dDebugPad_c dDebugPad;

#endif /* D_DEBUG_PAD_H */
