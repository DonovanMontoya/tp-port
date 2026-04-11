#pragma once
/**
 * port/include/d/d_kankyo_tev_str.h
 * PC port stub — defines dKy_tevstr_c with enough light-object layout to
 * satisfy actor/gameplay headers that take addresses of embedded GX lights.
 */
#ifndef D_D_KANKYO_TEV_STR
#define D_D_KANKYO_TEV_STR

#include "JSystem/J3DGraphBase/J3DTevs.h"
#include "SSystem/SComponent/c_xyz.h"
#include "port/types.h"

class dKy_tevstr_c {
public:
    /* 0x000 */ J3DLightObj mLightObj;
    /* 0x074 */ J3DLightObj mLights[6];
    // 0x32C
    cXyz field_0x32c;
    // 0x338
    cXyz mLightPosWorld;
    // 0x344
    f32  field_0x344;
    // 0x348 — GXColorS10 AmbCol (8 bytes)
    GXColorS10 AmbCol;
    // 0x350 — GXColorS10 FogCol (8 bytes)
    GXColorS10 FogCol;
    // 0x358 — GXColorS10 TevColor (8 bytes)
    GXColorS10 TevColor;
    // 0x360 — GXColor TevKColor (4 bytes)
    GXColor TevKColor;
    // 0x364 — GXColor mLightInf (4 bytes)
    GXColor mLightInf;
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
