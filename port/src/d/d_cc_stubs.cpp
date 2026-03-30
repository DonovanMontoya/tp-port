#include "d/d_cc_s.h"

bool dCcS::m_mtrl_hit_tbl[64] = {};

dCcMassS_Obj::dCcMassS_Obj() : mpObj(nullptr), mPriority(0), mpAreaCheckFunc(nullptr) {}

void dCcMassS_Obj::Set(cCcD_Obj* p_obj, u8 priority, dCcMassS_ObjCallback callback) {
    mpObj = p_obj;
    mPriority = priority;
    mpAreaCheckFunc = callback;
}

void dCcMassS_Obj::Clear() {
    mpObj = nullptr;
    mPriority = 0;
    mpAreaCheckFunc = nullptr;
}

void dCcMassS_HitInf::ClearPointer() {
    mpArea = nullptr;
    mpAtObj = nullptr;
    mpCoObj = nullptr;
    mCoHitLen = 0.0f;
}

dCcMassS_Mng::dCcMassS_Mng() { Ct(); }

void dCcMassS_Mng::Ct() {
    mMassObjCount = 0;
    mMassAreaCount = 0;
    field_0x200 = 0;
    field_0x201 = 0;
    field_0x202 = 0;
    mResultCam = 0;
    mCamTopPos.x = 0.0f;
    mCamTopPos.y = 0.0f;
    mCamTopPos.z = 0.0f;
    mCamTopDist = 0.0f;
    mCamBottomPos.x = 0.0f;
    mCamBottomPos.y = 0.0f;
    mCamBottomPos.z = 0.0f;
    mCamBottomDist = 0.0f;
}

void dCcMassS_Mng::SetAttr(f32 radius, f32 height, u8 param_2, u8 param_3) {
    mCamTopDist = radius;
    mCamBottomDist = height;
    field_0x200 = param_2;
    field_0x201 = param_3;
}

void dCcMassS_Mng::Prepare() {}
u32 dCcMassS_Mng::Chk(cXyz*, fopAc_ac_c**, dCcMassS_HitInf*) { return 0; }
void dCcMassS_Mng::Clear() { mMassObjCount = 0; mMassAreaCount = 0; }
void dCcMassS_Mng::Set(cCcD_Obj* p_obj, u8 priority) {
    if (mMassObjCount < 10) {
        mMassObjs[mMassObjCount++].Set(p_obj, priority, nullptr);
    }
}
void dCcMassS_Mng::SetCam(cM3dGCps const&) { mResultCam = 0; }
u32 dCcMassS_Mng::GetResultCam() const { return mResultCam; }
void dCcMassS_Mng::GetCamTopPos(Vec* p_out) {
    if (p_out != nullptr) {
        *p_out = mCamTopPos;
    }
}

void dCcS::Ct() {}
void dCcS::Dt() {}
bool dCcS::ChkShieldFrontRange(cCcD_Obj*, cCcD_Obj*, int, cXyz const*) { return false; }
bool dCcS::ChkShield(cCcD_Obj*, cCcD_Obj*, dCcD_GObjInf*, dCcD_GObjInf*, cXyz const*) { return false; }
void dCcS::CalcTgPlusDmg(cCcD_Obj*, cCcD_Obj*, cCcD_Stts*, cCcD_Stts*) {}
int dCcS::GetRank(u8) { return 0; }
void dCcS::ProcAtTgHitmark(bool, bool, cCcD_Obj*, cCcD_Obj*, dCcD_GObjInf*, dCcD_GObjInf*,
                           cCcD_Stts*, cCcD_Stts*, dCcD_GStts*, dCcD_GStts*, cXyz*, bool) {}
bool dCcS::ChkCamera(cXyz&, cXyz&, f32, fopAc_ac_c*, fopAc_ac_c*, fopAc_ac_c*) { return false; }
bool dCcS::chkCameraPoint(cXyz const&, cCcD_ShapeAttr::Shape*, fopAc_ac_c*, fopAc_ac_c*) {
    return false;
}
void dCcS::DrawAfter() {}
void dCcS::Move() {}
void dCcS::Draw() {}
void dCcS::MassClear() { mMass_Mng.Clear(); }
BOOL dCcS::ChkAtTgMtrlHit(u8, u8) { return false; }
void dCcS::SetPosCorrect(cCcD_Obj*, cXyz*, cCcD_Obj*, cXyz*, f32) {}
void dCcS::SetCoGObjInf(bool, bool, cCcD_GObjInf*, cCcD_GObjInf*, cCcD_Stts*, cCcD_Stts*,
                        cCcD_GStts*, cCcD_GStts*) {}
void dCcS::SetAtTgGObjInf(bool, bool, cCcD_Obj*, cCcD_Obj*, cCcD_GObjInf*, cCcD_GObjInf*,
                          cCcD_Stts*, cCcD_Stts*, cCcD_GStts*, cCcD_GStts*, cXyz*) {}
bool dCcS::ChkNoHitGAtTg(cCcD_GObjInf const*, cCcD_GObjInf const*, cCcD_GStts*, cCcD_GStts*) {
    return false;
}
bool dCcS::ChkAtTgHitAfterCross(bool, bool, cCcD_GObjInf const*, cCcD_GObjInf const*,
                                cCcD_Stts*, cCcD_Stts*, cCcD_GStts*, cCcD_GStts*) {
    return false;
}
bool dCcS::ChkNoHitGCo(cCcD_Obj*, cCcD_Obj*) { return false; }
void dCcS::MoveAfterCheck() {}
void dCcS::CalcParticleAngle(dCcD_GObjInf*, cCcD_Stts*, cCcD_Stts*, csXyz*) {}
void dCcS::ChkActor(fopAc_ac_c*) {}
