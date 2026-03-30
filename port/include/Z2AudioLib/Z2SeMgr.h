#pragma once
#ifndef Z2SEMGR_H
#define Z2SEMGR_H
#include "JSystem/JAudio2/JASGadget.h"

// Minimal subset of the Z2SE sound-ID enum needed by compiled files.
// Original has ~300+ entries; add only what's referenced by enabled source.
enum {
    Z2SE_TITLE_ENTER           = 0x3,
    Z2SE_SY_CURSOR_OPTION       = 0x4C,
    Z2SE_SY_TALK_WIN_OPEN       = 0x61,
    Z2SE_SY_TALK_WIN_CLOSE      = 0x62,
    Z2SE_SY_CURSOR_OK           = 0x6D,
    Z2SE_SY_CURSOR_CANCEL       = 0x6E,
    Z2SE_SY_MENU_CURSOR_COMMON  = 0xA0,
};

class Z2SeMgr : public JASGlobalInstance<Z2SeMgr> {
public:
    Z2SeMgr() : JASGlobalInstance<Z2SeMgr>(true) {}
};
#endif
