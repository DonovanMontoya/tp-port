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
    static void make_prm_bokkuri(u32* /*o_params*/, csXyz* /*o_paramsEx*/,
                                  u8 /*itemNo*/, u8 /*itemBit*/, u8 /*itemType*/, u8 /*p5*/) {}
    static void clrSaveFlag() {}
    static void setSaveFlag() {}
};

#endif /* D_A_OBJ_CARRY_H */
