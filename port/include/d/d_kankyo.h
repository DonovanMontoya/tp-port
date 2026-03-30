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

// Environment light class — used by drawBallModel in f_op_actor.cpp
class dScnKy_env_light_c {
public:
    void setLightTevColorType_MAJI(J3DModelData* /*md*/, dKy_tevstr_c* /*tev*/) {}
    void setLightTevColorType_MAJI(J3DModel* /*model*/, dKy_tevstr_c* /*tev*/) {}

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
inline void dKy_GfFog_tevstr_set(dKy_tevstr_c*) {}
inline void dKy_setLight_mine(dKy_tevstr_c*) {}
inline void dKy_setLight_again() {}
inline void dKy_GxFog_set() {}
inline void dKy_Global_amb_set(dKy_tevstr_c*) {}
inline BOOL dKy_darkworld_check() { return FALSE; }
inline BOOL dKy_daynight_check() { return FALSE; }
inline BOOL dKy_darkworld_stage_check(const char*, int) { return FALSE; }
inline BOOL dKy_darkworld_spot_check(const char*, int) { return FALSE; }
inline void dKy_darkworld_Area_set(const char*, int) {}
inline dScnKy_env_light_c* dKy_getEnvlight() { return &g_env_light; }
inline int dKy_getdaytime_hour() { return 6; }
inline int dKy_getdaytime_minute() { return 0; }
inline int dKy_get_dayofweek() { return 0; }
inline void dKy_itudemo_se() {}

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
    char _pad[0x18];
};

inline void dKy_bg1_addcol_amb_set(s16, s16, s16, f32) {}
inline void dKy_vrbox_addcol_sky0_set(s16, s16, s16, f32) {}
inline void dKy_vrbox_addcol_kasumi_set(s16, s16, s16, f32) {}
inline void dKy_addcol_fog_set(s16, s16, s16, f32) {}
inline cXyz dKy_light_influence_pos(int) { return cXyz(0.0f, 0.0f, 0.0f); }
inline void dKy_bgparts_activelight_set(LIGHT_INFLUENCE*, int) {}
inline void dKy_bgparts_activelight_cut(int) {}
inline void dKy_mock_light_every_set(LIGHT_INFLUENCE*) {}
inline void dKy_camera_water_in_status_set(u8) {}
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

#endif /* D_KANKYO_D_KANKYO_H */
