#pragma once
/**
 * port/include/d/d_bg_s.h
 * PC port stub — background/collision system.
 * Shadows the real d_bg_s.h. All checks return safe "no hit" defaults.
 */
#ifndef D_BG_D_BG_S_H
#define D_BG_D_BG_S_H

#include "port/types.h"
#include "SSystem/SComponent/c_xyz.h"
#include "SSystem/SComponent/c_m3d_g_pla.h"
#include "SSystem/SComponent/c_bg_s_poly_info.h"
#include "SSystem/SComponent/c_bg_s_chk.h"
#include "SSystem/SComponent/c_bg_s_gnd_chk.h"
#include "SSystem/SComponent/c_bg_s_lin_chk.h"

class fopAc_ac_c;
class dBgW_Base;
class dBgS_Acch;
class dBgS_LinChk;
class dBgS_GndChk;
class dBgS_RoofChk;
class dBgS_SplGrpChk;
class dBgS_ShdwDraw;

// cBgS — fully inline port stub, no external vtable needed
class cBgS {
public:
    cBgS()  {}
    virtual ~cBgS() {}
    virtual void Ct() {}
    virtual void Dt() {}

    bool Regist(dBgW_Base*, fpc_ProcID, void*)                     { return false; }
    bool Release(dBgW_Base*)                                        { return false; }
    bool LineCross(cBgS_LinChk*)                                    { return false; }
    f32  GroundCross(cBgS_GndChk*)                                  { return -99999.0f; }
    fopAc_ac_c* GetActorPointer(int) const                          { return nullptr; }
    dBgW_Base*  GetBgWBasePointer(cBgS_PolyInfo const&) const       { return nullptr; }
    bool ChkPolySafe(cBgS_PolyInfo const&)                          { return false; }
    s32  GetGrpRoomId(cBgS_PolyInfo const&) const                   { return -1; }
    bool GetTriPla(cBgS_PolyInfo const&, cM3dGPla*) const          { return false; }
    bool GetTriPnt(cBgS_PolyInfo const&, cXyz*, cXyz*, cXyz*) const { return false; }
    void ShdwDraw(dBgS_ShdwDraw*)                                   {}
    u32  GetGrpInf(cBgS_PolyInfo const&) const                     { return 0; }
    void Draw()                                                     {}

    fopAc_ac_c* GetActorPointer(cBgS_PolyInfo const& info) const {
        return GetActorPointer(info.GetBgIndex());
    }
};

// dBgS — game-level background system stub
class dBgS : public cBgS {
public:
    dBgS()  {}
    virtual ~dBgS() {}

    bool LineCross(dBgS_LinChk*)                                   { return false; }
    f32  GroundCross(dBgS_GndChk*)                                 { return -99999.0f; }
    f32  RoofChk(dBgS_RoofChk*)                                   { return -99999.0f; }
    bool SplGrpChk(dBgS_SplGrpChk*)                               { return false; }
    bool WaterChk(dBgS_SplGrpChk* chk)                            { return SplGrpChk(chk); }

    // Attribute/poly accessors
    int  GetPolyAtt0(cBgS_PolyInfo const&)                         { return 0; }
    int  GetPolyAtt1(cBgS_PolyInfo const&)                         { return 0; }
    s32  GetPolyColor(cBgS_PolyInfo const&)                        { return 0; }
    BOOL GetHorseNoEntry(cBgS_PolyInfo const&)                     { return FALSE; }
    s32  GetWallCode(cBgS_PolyInfo const&)                         { return 0; }
    int  GetRoomId(cBgS_PolyInfo const&)                           { return -1; }
    bool ChkMoveBG(cBgS_PolyInfo const&)                           { return false; }
    bool ChkMoveBG_NoDABg(cBgS_PolyInfo const&)                    { return false; }
    bool GetPolyAttackThrough(cBgS_PolyInfo const&)                { return false; }
    bool CheckRoofChk(dBgS_GndChk*)                                { return false; }
};

// Background system global accessor — defined in game_stubs.cpp
dBgS& dComIfG_Bgsp();

// Inline helper functions for wall/ground normals (from c_bg_w.h)
inline bool cBgW_CheckBWall(float normalY)   { (void)normalY; return false; }
inline bool cBgW_CheckBGround(float normalY) { (void)normalY; return true; }

#endif /* D_BG_D_BG_S_H */
