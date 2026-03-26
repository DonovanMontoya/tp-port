#pragma once
/**
 * port/include/d/d_kankyo_tev_str.h
 * PC port stub — defines dKy_tevstr_c without J3DTevs/GX dependencies.
 *
 * The original struct is 0x388 bytes with J3DLightObj members (0x74 each).
 * We preserve the layout so that fopAc_ac_c offsets remain consistent.
 * field_0x32c (cXyz at offset 0x32C) is the only member accessed in
 * non-debug f_op_actor_mng.cpp code.
 */
#ifndef D_D_KANKYO_TEV_STR
#define D_D_KANKYO_TEV_STR

#include "SSystem/SComponent/c_xyz.h"
#include "port/types.h"

class dKy_tevstr_c {
public:
    // J3DLightObj mLightObj at 0x000 (size 0x74)
    char _pad_lightobj[0x74];
    // J3DLightObj mLights[6] at 0x074 (size 6 * 0x74 = 0x2B8)
    char _pad_lights[0x2B8];
    // 0x32C
    cXyz field_0x32c;
    // 0x338
    cXyz mLightPosWorld;
    // 0x344
    f32  field_0x344;
    // 0x348 — GXColorS10 AmbCol (8 bytes)
    char _pad_amb[8];
    // 0x350 — GXColorS10 FogCol (8 bytes)
    char _pad_fog[8];
    // 0x358 — GXColorS10 TevColor (8 bytes)
    char _pad_tev[8];
    // 0x360 — GXColor TevKColor (4 bytes)
    char _pad_tevk[4];
    // 0x364 — GXColor mLightInf (4 bytes)
    char _pad_linf[4];
    // 0x368
    f32  mFogStartZ;
    f32  mFogEndZ;
    f32  pat_ratio;
    f32  field_0x374;
    // 0x378
    u16  Material_id;
    u8   Type;
    u8   mInitTimer;
    u8   UseCol;
    u8   PrevCol;
    u8   wether_pat1;
    u8   wether_pat0;
    s8   room_no;
    u8   YukaCol;
    u8   mLightMode;
    u8   Material_use_fg;
    u8   field_0x384;
    u8   field_0x385;
    // padding to reach 0x388
    char _tail[2];
};  // Size = 0x388

#endif /* D_D_KANKYO_TEV_STR */
