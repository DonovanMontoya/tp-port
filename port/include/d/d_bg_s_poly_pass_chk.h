#pragma once
#ifndef D_BG_D_BG_S_POLY_PASS_CHK_H
#define D_BG_D_BG_S_POLY_PASS_CHK_H

#include "SSystem/SComponent/c_bg_s_poly_pass_chk.h"

class dBgS_PolyPassChk : public cBgS_PolyPassChk {
public:
    dBgS_PolyPassChk()  {}
    virtual ~dBgS_PolyPassChk() {}

    bool ChkArrow()         { return mArrow; }
    bool ChkBomb()          { return mBomb; }
    bool ChkBoomerang()     { return mBoomerang; }
    bool ChkCam()           { return mCamera; }
    bool ChkHorse()         { return mHorse; }
    bool ChkIronBall()      { return mIronBall; }
    bool ChkLink()          { return mLink; }
    bool ChkNoHorse()       { return false; }
    bool ChkObj()           { return mObject; }
    bool ChkRope()          { return mRope; }
    bool ChkStatue()        { return mStatue; }
    bool ChkUnderwaterRoof(){ return mUnderwaterRoof; }

    void ClrBomb()          { mBomb = false; }
    void ClrCam()           { mCamera = false; }
    void ClrIronBall()      { mIronBall = false; }
    void ClrLink()          { mLink = false; }
    void ClrObj()           { mObject = false; }
    void ClrStatue()        { mStatue = false; }

    void SetArrow()         { mArrow = true; }
    void SetBomb()          { mBomb = true; }
    void SetBoomerang()     { mBoomerang = true; }
    void SetCam()           { mCamera = true; }
    void SetHorse()         { mHorse = true; }
    void SetIronBall()      { mIronBall = true; }
    void SetLink()          { mLink = true; }
    void SetObj()           { mObject = true; }
    void SetPassChkInfo(dBgS_PolyPassChk&) {}
    void SetRope()          { mRope = true; }
    void SetStatue()        { mStatue = true; }
    void SetUnderwaterRoof(){ mUnderwaterRoof = true; }
    void SetObj(bool v)     { mObject = v; }

private:
    bool mObject = false;
    bool mCamera = false;
    bool mLink = false;
    bool mArrow = false;
    bool mBomb = false;
    bool mBoomerang = false;
    bool mRope = false;
    bool mUnderwaterRoof = false;
    bool mHorse = false;
    bool mStatue = false;
    bool mIronBall = false;
};

#endif /* D_BG_D_BG_S_POLY_PASS_CHK_H */
