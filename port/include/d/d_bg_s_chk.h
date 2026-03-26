#pragma once
#ifndef D_BG_D_BG_S_CHK_H
#define D_BG_D_BG_S_CHK_H

#include "d/d_bg_s_poly_pass_chk.h"
#include "d/d_bg_s_grp_pass_chk.h"

class dBgS_Chk : public dBgS_PolyPassChk, public dBgS_GrpPassChk {
public:
    dBgS_Chk()  {}
    virtual ~dBgS_Chk() {}

    cBgS_PolyPassChk* GetPolyPassChkInfo() { return this; }
    cBgS_GrpPassChk*  GetGrpPassChkInfo()  { return this; }
    void SetObj() { dBgS_PolyPassChk::SetObj(); }
};

#endif /* D_BG_D_BG_S_CHK_H */
