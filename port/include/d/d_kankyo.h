#pragma once
/**
 * port/include/d/d_kankyo.h
 * PC port stub — kankyo (environment) system declarations.
 * Shadows the original which pulls in J3DStruct/GX and m_Do_graphic chains.
 * On PC all environment calculations are no-ops.
 */
#ifndef D_KANKYO_D_KANKYO_H
#define D_KANKYO_D_KANKYO_H

#include "port/types.h"
#include "d/d_kankyo_tev_str.h"
#include "f_pc/f_pc_base.h"
#include "SSystem/SComponent/c_xyz.h"

class fopAc_ac_c;
class J3DModel;
class J3DModelData;
class cBgS_PolyInfo;

// Environment light class — used by drawBallModel in f_op_actor.cpp
class dScnKy_env_light_c {
public:
    void setLightTevColorType_MAJI(J3DModelData* /*md*/, dKy_tevstr_c* /*tev*/) {}
    void setLightTevColorType_MAJI(J3DModel* /*model*/, dKy_tevstr_c* /*tev*/) {}
    void settingTevStruct(int, cXyz const*, dKy_tevstr_c*) {}

    GXColorS10 actor_amb_col = {0, 0, 0, 0};
    GXColorS10 bg_amb_col[4] = {};
    f32 bg_light_influence = 0.0f;
    f32 field_0x1264 = 0.0f;
    f32 field_0x126c = 1000000.0f;
    f32 mDemoAttentionPoint = 0.0f;
    u8 light_mask_type = 0;
    u8 fishing_hole_season = 0;
    u8 now_senses_effect = 0;
    f32 senses_effect_strength = 0.0f;
    u8 staffroll_next_timer = 0;
    bool is_blure = false;
    bool camera_water_in_status = false;
};
extern dScnKy_env_light_c g_env_light;

// Environment calculation stubs
inline void dKy_tevstr_init(dKy_tevstr_c* /*tevstr*/, s8 /*room_no*/, u8 /*floorCol*/) {}
inline void dKy_depth_dist_set(void* /*process*/) {}

// Light/fog stubs
inline void dKy_setLight(dKy_tevstr_c*) {}
inline void dKy_setLight() {}
inline void dKy_setLight_nowroom(char) {}
inline void dKy_setLight_nowroom_grass(char, f32) {}
inline void dKy_GfFog_tevstr_set(dKy_tevstr_c*) {}
inline void dKy_GxFog_tevstr_set(dKy_tevstr_c* tev) { dKy_GfFog_tevstr_set(tev); }
inline void dKy_setLight_mine(dKy_tevstr_c*) {}
inline void dKy_setLight_again() {}
inline void dKy_GxFog_set() {}
inline void dKy_Global_amb_set(dKy_tevstr_c*) {}
inline BOOL dKy_darkworld_check() { return FALSE; }
inline BOOL dKy_daynight_check() { return FALSE; }
inline BOOL dKy_darkworld_stage_check(const char*, int) { return FALSE; }
inline BOOL dKy_darkworld_spot_check(const char*, int) { return FALSE; }
inline void dKy_darkworld_Area_set(const char*, int) {}
inline BOOL dKy_withwarp_capture_check() { return FALSE; }
inline dScnKy_env_light_c* dKy_getEnvlight() { return &g_env_light; }
inline int dKy_getdaytime_hour() { return 6; }
inline int dKy_getdaytime_minute() { return 0; }
inline int dKy_get_dayofweek() { return 0; }
inline void dKy_itudemo_se() {}
inline void dKy_set_nexttime(f32) {}

// Misc kankyo stubs
inline int  dKy_Indoor_check() { return 0; }
inline u8   dKy_get_schbit() { return 0; }
inline int  dKy_get_schbit_timer() { return 0; }
inline BOOL dKy_SunMoon_Light_Check() { return FALSE; }
inline f32  dKy_move_room_ratio(dKy_tevstr_c*, s8*) { return 0.0f; }

// Wind/weather
struct WIND_INFLUENCE {
    cXyz position;
    cXyz mDirection;
    f32  mRadius;
    f32  mStrength;
    f32  field_0x20;
};

struct LIGHT_INFLUENCE {
    cXyz mPosition;
    GXColorS10 mColor = {0, 0, 0, 0};
    f32 mPow = 0.0f;
    f32 mFluctuation = 0.0f;
    int mIndex = -1;
};

inline void dKy_bg1_addcol_amb_set(s16, s16, s16, f32) {}
inline void dKy_vrbox_addcol_sky0_set(s16, s16, s16, f32) {}
inline void dKy_vrbox_addcol_kasumi_set(s16, s16, s16, f32) {}
inline void dKy_addcol_fog_set(s16, s16, s16, f32) {}
inline cXyz dKy_light_influence_pos(int) { return cXyz(0.0f, 0.0f, 0.0f); }
inline void dKy_bgparts_activelight_set(LIGHT_INFLUENCE*, int) {}
inline void dKy_bgparts_activelight_cut(int) {}
inline void dKy_mock_light_every_set(LIGHT_INFLUENCE*) {}
inline void dKy_ParticleColor_get_actor(cXyz*, dKy_tevstr_c*, GXColor* c0, GXColor* c1,
                                        GXColor* fallback0, GXColor* fallback1, f32) {
    if (c0 && fallback0) {
        *c0 = *fallback0;
    }
    if (c1 && fallback1) {
        *c1 = *fallback1;
    }
}
inline void dKy_ParticleColor_get_bg(cXyz*, dKy_tevstr_c*, GXColor* c0, GXColor* c1,
                                     GXColor* fallback0, GXColor* fallback1, f32) {
    if (c0 && fallback0) {
        *c0 = *fallback0;
    }
    if (c1 && fallback1) {
        *c1 = *fallback1;
    }
}
inline GXColor dKy_light_influence_col(GXColor* color, f32) {
    return color ? *color : GXColor{0xff, 0xff, 0xff, 0xff};
}
inline void dKy_camera_water_in_status_set(u8) {}
inline int dKy_camera_water_in_status_check() { return 0; }
inline void dKy_WaterIn_Light_set() {}
inline void dKy_SordFlush_set(cXyz, int) {}

struct tpPortKankyoNavyStub {
    bool field_0x30d = false;
};
struct tpPortKankyoHIOStub {
    tpPortKankyoNavyStub navy;
};
inline tpPortKankyoHIOStub g_kankyoHIO = {};

// Sound influence (used by fopAcM actor manager for reverb/sound zone)
struct SND_INFLUENCE {
    /* 0x00 */ cXyz       position;
    /* 0x0C */ int        field_0xc;
    /* 0x10 */ int        timer;
    /* 0x14 */ fpc_ProcID actor_id;
};
inline SND_INFLUENCE* dKy_Sound_get()     { return nullptr; }
inline SND_INFLUENCE* dKy_pol_sound_get() { return nullptr; }
inline u32 dKy_pol_sound_get(const void*) { return 0; }
inline u32 dKy_pol_argument_get(const void*) { return 0; }
inline void dKy_pol_eff_prim_get(const cBgS_PolyInfo*, GXColor* out) {
    if (out) *out = GXColor{0xff, 0xff, 0xff, 0xff};
}
inline void dKy_pol_eff_env_get(const cBgS_PolyInfo*, GXColor* out) {
    if (out) *out = GXColor{0xff, 0xff, 0xff, 0xff};
}
inline u8 dKy_pol_eff_alpha_get(const cBgS_PolyInfo*) { return 0xff; }
inline f32 dKy_pol_eff_ratio_get(const cBgS_PolyInfo*) { return 1.0f; }
inline void dKy_pol_eff2_prim_get(const cBgS_PolyInfo*, GXColor* out) {
    if (out) *out = GXColor{0xff, 0xff, 0xff, 0xff};
}
inline void dKy_pol_eff2_env_get(const cBgS_PolyInfo*, GXColor* out) {
    if (out) *out = GXColor{0xff, 0xff, 0xff, 0xff};
}
inline u8 dKy_pol_eff2_alpha_get(const cBgS_PolyInfo*) { return 0xff; }
inline f32 dKy_pol_eff2_ratio_get(const cBgS_PolyInfo*) { return 1.0f; }
inline u8 dKy_pol_efftype_get(const cBgS_PolyInfo*) { return 0; }
inline u8 dKy_pol_efftype2_get(const cBgS_PolyInfo*) { return 0; }
inline void dKy_Sound_set(cXyz, int, fpc_ProcID, int) {}
inline void dKy_Itemgetcol_chg_on() {}
inline void dKy_plight_set(LIGHT_INFLUENCE*) {}
inline void dKy_plight_cut(LIGHT_INFLUENCE*) {}
inline void dKy_shadow_mode_set(u8) {}
inline void dKy_shadow_mode_reset(u8) {}
inline int dKy_WolfEyeLight_set(cXyz*, f32, f32, f32, GXColor*, f32, u8, u8) { return 0; }
inline f32 dKyw_get_wind_pow() { return 0.0f; }
inline cXyz* dKyw_get_wind_vec() {
    static cXyz zero(0.0f, 0.0f, 0.0f);
    return &zero;
}
inline void dKyw_get_AllWind_vec(const cXyz*, cXyz* out_dir, f32* out_pow) {
    if (out_dir) {
        *out_dir = cXyz(0.0f, 0.0f, 0.0f);
    }
    if (out_pow) {
        *out_pow = 0.0f;
    }
}
inline int dKy_TeachWind_existence_chk() { return 0; }

#endif /* D_KANKYO_D_KANKYO_H */
