#pragma once
/**
 * port/include/d/actor/d_a_obj_carry.h
 * PC port stub — carry/throw actor data.
 * Shadows the original which pulls in d_a_player.h → d_drawlist.h chain.
 */
#ifndef D_A_OBJ_CARRY_H
#define D_A_OBJ_CARRY_H

#include "port/types.h"
#include "f_op/f_op_actor.h"
#include "SSystem/SComponent/c_sxyz.h"

struct daObjCarry_dt_t {
    f32 m_throwInitSpeedY;
    f32 m_throwInitSpeedXZ;
    f32 m_wolfThrowInitSpeedY;
    f32 m_wolfThrowInitSpeedXZ;
    f32 field_0x10;
    f32 field_0x14;
    f32 m_gravity;
    f32 m_Buoyancy;
    f32 m_urnWindEffRatio;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 scale;
    u8  m_carry_attn_dist;
    u8  m_flags;
};

struct daObjCarry_prm_t {};
struct daObjCarry_prm_ex_t {};

class daObjCarry_c : public fopAc_ac_c {
public:
    enum {
        TYPE_BALL_S = 0,
        TYPE_BALL_S_2 = 1,
        TYPE_OOTSUBO = 2,
        TYPE_AOTSUBO = 3,
        TYPE_TARU = 4,
    };

    static void make_prm_bokkuri(u32* /*o_params*/, csXyz* /*o_paramsEx*/,
                                  u8 /*itemNo*/, u8 /*itemBit*/, u8 /*itemType*/, u8 /*p5*/) {}
    static void clrSaveFlag();
    static void setSaveFlag();
    static bool chkSaveFlag();
    static const cXyz& getPos(int);
    static void savePos(int, cXyz);
    static void onSttsFlag(int, u8);
    static void offSttsFlag(int, u8);
    static u8 chkSttsFlag(int, u8);
    static void setRoomNo(int, s8);
    static s8 getRoomNo(int);
    int getType() const { return TYPE_BALL_S; }
    bool prm_chk_type_ironball() const { return false; }

    static bool mSaveFlag;
    static cXyz mPos[5];
    static u8 mSttsFlag[5];
    static s8 mRoomNo[5];
};

#endif /* D_A_OBJ_CARRY_H */
