#pragma once
#ifndef D_BG_D_BG_S_GRP_PASS_CHK_H
#define D_BG_D_BG_S_GRP_PASS_CHK_H

#include "SSystem/SComponent/c_bg_s_chk.h"

class dBgS_GrpPassChk : public cBgS_GrpPassChk {
public:
    enum { NORMAL_GRP = 1, WATER_GRP = 2, FULL_GRP = 3 };

    dBgS_GrpPassChk()  : mGrp(0) {}
    virtual ~dBgS_GrpPassChk() {}

    void OnWaterGrp()    { mGrp |= WATER_GRP; }
    void OnSpl()         { mGrp |= WATER_GRP; }
    void OffWaterGrp()   { mGrp &= ~WATER_GRP; }
    void OnNormalGrp()   { mGrp |= NORMAL_GRP; }
    void OffNormalGrp()  { mGrp &= ~NORMAL_GRP; }
    void OnFullGrp()     { mGrp |= FULL_GRP; }
    void OffFullGrp()    { mGrp &= ~FULL_GRP; }
    void OnAll()         { mGrp |= FULL_GRP; }
    u32 MaskNormalGrp()  { return mGrp & NORMAL_GRP; }
    u32 MaskWaterGrp()   { return mGrp & WATER_GRP; }
private:
    u32 mGrp;
};

#endif /* D_BG_D_BG_S_GRP_PASS_CHK_H */
