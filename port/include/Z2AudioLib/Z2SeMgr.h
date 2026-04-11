#pragma once

#if TP_PORT_USE_REAL_ALINK
#include "../../../include/Z2AudioLib/Z2SeMgr.h"
static constexpr u32 Z2BGM_OPEN_BOX = 0x1000012;
static constexpr u32 Z2BGM_GAME_OVER = 0x1000013;
static constexpr u32 Z2BGM_ITEM_GET = 0x100000A;
static constexpr u32 Z2BGM_ITEM_GET_MINI = 0x100000B;
static constexpr u32 Z2BGM_ITEM_GET_ME = 0x1000014;
static constexpr u32 Z2BGM_HEART_GET = 0x100001C;
static constexpr u32 Z2BGM_ITEM_GET_INSECT = 0x1000066;
static constexpr u32 Z2BGM_ITEM_GET_SMELL = 0x1000067;
static constexpr u32 Z2BGM_ITEM_GET_POU = 0x1000088;
static constexpr u32 Z2BGM_ITEM_GET_ME_S = 0x1000094;
#else

#include "JSystem/JAudio2/JASGadget.h"
#include "port/types.h"

// Minimal subset of the Z2SE sound-ID enum needed by the broad default build.
enum Z2SoundID : u32 {
    Z2SE_TITLE_ENTER          = 0x3,
    Z2SE_SUBJ_VIEW_IN         = 0x37,
    Z2SE_SUBJ_VIEW_OUT        = 0x38,
    Z2SE_SY_CURSOR_OPTION     = 0x4C,
    Z2SE_SY_TALK_WIN_OPEN     = 0x61,
    Z2SE_SY_TALK_WIN_CLOSE    = 0x62,
    Z2SE_SY_CURSOR_OK         = 0x6D,
    Z2SE_SY_CURSOR_CANCEL     = 0x6E,
    Z2SE_SY_MENU_CURSOR_COMMON = 0xA0,
    Z2SE_AL_CANOE_FALL_DW     = 0x1000,
    Z2SE_HIT_SWORD            = 0x40000,
    Z2SE_HIT_SWORD_STAB       = 0x40001,
    Z2SE_HIT_SHIELD_ATTACK    = 0x40002,
    Z2SE_HIT_WOLFBITE         = 0x40003,
    Z2SE_HIT_WOLFKICK         = 0x40004,
    Z2SE_HIT_WOOD_WEAPON      = 0x40005,
    Z2SE_HIT_HAMMER           = 0x40006,
    Z2SE_HIT_ARROW_STICK      = 0x40007,
    Z2SE_HIT_ARROW_REBOUND    = 0x40008,
    Z2SE_HIT_HOOKSHOT_STICK   = 0x40009,
    Z2SE_HIT_HOOKSHOT_REBOUND = 0x4000A,
    Z2SE_HIT_METAL_WEAPON     = 0x4000B,
    Z2SE_HIT_STONE_WEAPON     = 0x4000C,
    Z2SE_HIT_HARD_BODY        = 0x4000D,
    Z2SE_HIT_SOFT_BODY        = 0x4000E,
    Z2SE_HIT_DUMMY_CANCEL     = 0x4000F,
    Z2SE_HIT_SWORD_RUN        = 0x40010,
    Z2SE_HIT_EN_WOOD_WEAPON   = 0x40011,
    Z2SE_HIT_THOROW_OBJ       = 0x40012,
    Z2SE_HIT_SPINNER          = 0x40013,
    Z2SE_HIT_COPYROD          = 0x40014,
    Z2SE_HIT_STATUE_HAMMER    = 0x40015,
    Z2SE_HIT_AL_ARROW_STICK   = 0x40016,
    Z2SE_HIT_AL_ARROW_REBOUND = 0x40017,
    Z2SE_HIT_PACHINKO         = 0x40018,
};

class Z2SeMgr : public JASGlobalInstance<Z2SeMgr> {
public:
    Z2SeMgr() : JASGlobalInstance<Z2SeMgr>(true) {}
};

#endif
