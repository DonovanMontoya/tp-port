#pragma once
#ifndef D_BG_D_BG_S_LIN_CHK_H
#define D_BG_D_BG_S_LIN_CHK_H

#include "SSystem/SComponent/c_bg_s_lin_chk.h"
#include "d/d_bg_s_chk.h"

class fopAc_ac_c;

class dBgS_LinChk : public cBgS_LinChk, public dBgS_Chk {
public:
    dBgS_LinChk()  {}
    virtual ~dBgS_LinChk() {}
    void Set(cXyz const*, cXyz const*, fopAc_ac_c const*) {}
};

class dBgS_ObjLinChk : public dBgS_LinChk {
public: dBgS_ObjLinChk() {} virtual ~dBgS_ObjLinChk() {}
};
class dBgS_LinkLinChk : public dBgS_LinChk {
public: dBgS_LinkLinChk() {} virtual ~dBgS_LinkLinChk() {}
};
class dBgS_RopeLinChk : public dBgS_LinChk {
public: dBgS_RopeLinChk() {} virtual ~dBgS_RopeLinChk() {}
};
class dBgS_BoomerangLinChk : public dBgS_LinChk {
public: dBgS_BoomerangLinChk() {} virtual ~dBgS_BoomerangLinChk() {}
};
class dBgS_ArrowLinChk : public dBgS_LinChk {
public: dBgS_ArrowLinChk() {} virtual ~dBgS_ArrowLinChk() {}
};
class dBgS_BombLinChk : public dBgS_LinChk {
public: dBgS_BombLinChk() {} virtual ~dBgS_BombLinChk() {}
};
class dBgS_HorseLinChk : public dBgS_LinChk {
public: dBgS_HorseLinChk() {} virtual ~dBgS_HorseLinChk() {}
};
class dBgS_CamLinChk : public dBgS_LinChk {
public: dBgS_CamLinChk() {} virtual ~dBgS_CamLinChk() {}
};
class dBgS_CamLinChk_NorWtr : public dBgS_CamLinChk {
public: dBgS_CamLinChk_NorWtr() {} virtual ~dBgS_CamLinChk_NorWtr() {}
};

#endif /* D_BG_D_BG_S_LIN_CHK_H */
