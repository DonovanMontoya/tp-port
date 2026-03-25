#pragma once
/**
 * PC stub/pass-through for dolphin/mtx.h
 * The types and macros are all standard C — just redirect types.h.
 * The actual implementation is in src/dolphin/mtx.cpp.
 */
#include "port/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Vec { f32 x, y, z; } Vec, *VecPtr;
typedef Vec Point3d, *Point3dPtr;
typedef struct { s16 x, y, z; } S16Vec, *S16VecPtr;
typedef struct { f32 x, y, z, w; } Quaternion, *QuaternionPtr, Qtrn, *QtrnPtr;

typedef f32 Mtx[3][4];
typedef f32 (*MtxPtr)[4];
typedef f32 Mtx44[4][4];
typedef f32 (*Mtx44Ptr)[4];
typedef f32 ROMtx[4][3];
typedef f32 (*ROMtxPtr)[4];
typedef f32 Mtx33[3][3];
typedef f32 Mtx23[2][3];
typedef f32 (*Mtx3P)[3];
typedef f32 (*MtxP)[4];
typedef const f32 (*CMtxP)[4];
typedef f32 QuaternionP[4];

typedef struct { u32 numMtx; MtxPtr stackBase; MtxPtr stackPtr; } MTXStack;

#define MTXDegToRad(d) ((d) * 0.01745329252f)
#define MTXRadToDeg(r) ((r) * 57.29577951f)

// -- Mtx (3x4) ---
void C_MTXIdentity(Mtx m);
void C_MTXCopy(const Mtx src, Mtx dst);
void C_MTXConcat(const Mtx a, const Mtx b, Mtx ab);
void C_MTXConcatArray(const Mtx a, const Mtx* srcBase, Mtx* dstBase, u32 count);
void C_MTXTranspose(const Mtx src, Mtx xPose);
u32  C_MTXInverse(const Mtx src, Mtx inv);
u32  C_MTXInvXpose(const Mtx src, Mtx invX);
void C_MTXRotRad(Mtx m, char axis, f32 rad);
void C_MTXRotTrig(Mtx m, char axis, f32 sinA, f32 cosA);
void C_MTXRotAxisRad(Mtx m, const Vec* axis, f32 rad);
void C_MTXTrans(Mtx m, f32 xT, f32 yT, f32 zT);
void C_MTXTransApply(const Mtx src, Mtx dst, f32 xT, f32 yT, f32 zT);
void C_MTXScale(Mtx m, f32 xS, f32 yS, f32 zS);
void C_MTXScaleApply(const Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS);
void C_MTXQuat(Mtx m, const Quaternion* q);
void C_MTXReflect(Mtx m, const Vec* p, const Vec* n);
void C_MTXLookAt(Mtx m, const Point3d* camPos, const Vec* camUp, const Point3d* target);
void C_MTXLightFrustum(Mtx m,f32 t,f32 b,f32 l,f32 r,f32 n,f32 sS,f32 sT,f32 tS,f32 tT);
void C_MTXLightPerspective(Mtx m,f32 fovY,f32 aspect,f32 sS,f32 sT,f32 tS,f32 tT);
void C_MTXLightOrtho(Mtx m,f32 t,f32 b,f32 l,f32 r,f32 sS,f32 sT,f32 tS,f32 tT);

// PS versions alias to C versions on PC
#define PSMTXIdentity    C_MTXIdentity
#define PSMTXCopy        C_MTXCopy
#define PSMTXConcat      C_MTXConcat
#define PSMTXConcatArray C_MTXConcatArray
#define PSMTXTranspose   C_MTXTranspose
#define PSMTXInverse     C_MTXInverse
#define PSMTXInvXpose    C_MTXInvXpose
#define PSMTXRotRad      C_MTXRotRad
#define PSMTXRotTrig     C_MTXRotTrig
#define PSMTXRotAxisRad  C_MTXRotAxisRad
#define PSMTXTrans       C_MTXTrans
#define PSMTXTransApply  C_MTXTransApply
#define PSMTXScale       C_MTXScale
#define PSMTXScaleApply  C_MTXScaleApply
#define PSMTXQuat        C_MTXQuat
#define PSMTXReflect     C_MTXReflect

// Always use optimised (PS) versions on PC
#define MTXIdentity    PSMTXIdentity
#define MTXCopy        PSMTXCopy
#define MTXConcat      PSMTXConcat
#define MTXInverse     PSMTXInverse
#define MTXTranspose   PSMTXTranspose
#define MTXInvXpose    PSMTXInvXpose
#define MTXRotRad      PSMTXRotRad
#define MTXRotTrig     PSMTXRotTrig
#define MTXRotAxisRad  PSMTXRotAxisRad
#define MTXTrans       PSMTXTrans
#define MTXTransApply  PSMTXTransApply
#define MTXScale       PSMTXScale
#define MTXScaleApply  PSMTXScaleApply
#define MTXQuat        PSMTXQuat
#define MTXReflect     PSMTXReflect
#define MTXLookAt           C_MTXLookAt
#define MTXLightFrustum     C_MTXLightFrustum
#define MTXLightPerspective C_MTXLightPerspective
#define MTXLightOrtho       C_MTXLightOrtho

// -- Mtx vec ---
void C_MTXMultVec(const Mtx m, const Vec* src, Vec* dst);
void C_MTXMultVecArray(const Mtx m, const Vec* srcBase, Vec* dstBase, u32 count);
void C_MTXMultVecSR(const Mtx m, const Vec* src, Vec* dst);
void C_MTXMultVecArraySR(const Mtx m, const Vec* srcBase, Vec* dstBase, u32 count);
#define PSMTXMultVec        C_MTXMultVec
#define PSMTXMultVecArray   C_MTXMultVecArray
#define PSMTXMultVecSR      C_MTXMultVecSR
#define PSMTXMultVecArraySR C_MTXMultVecArraySR
#define MTXMultVec          C_MTXMultVec
#define MTXMultVecArray     C_MTXMultVecArray
#define MTXMultVecSR        C_MTXMultVecSR
#define MTXMultVecArraySR   C_MTXMultVecArraySR

// -- Mtx44 ---
void C_MTX44Identity(Mtx44 m);
void C_MTX44Copy(const Mtx44 src, Mtx44 dst);
void C_MTX44Concat(const Mtx44 a, const Mtx44 b, Mtx44 ab);
void C_MTX44Transpose(const Mtx44 src, Mtx44 xPose);
void C_MTX44Trans(Mtx44 m, f32 xT, f32 yT, f32 zT);
void C_MTX44TransApply(const Mtx44 src, Mtx44 dst, f32 xT, f32 yT, f32 zT);
void C_MTX44Scale(Mtx44 m, f32 xS, f32 yS, f32 zS);
void C_MTX44ScaleApply(const Mtx44 src, Mtx44 dst, f32 xS, f32 yS, f32 zS);
void C_MTX44RotRad(Mtx44 m, char axis, f32 rad);
void C_MTX44RotTrig(Mtx44 m, char axis, f32 sinA, f32 cosA);
void C_MTX44RotAxisRad(Mtx44 m, const Vec* axis, f32 rad);
void C_MTXFrustum(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);
void C_MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f);
void C_MTXOrtho(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);
u32  C_MTX44Inverse(const Mtx44 src, Mtx44 inv);

#define PSMTX44Identity    C_MTX44Identity
#define PSMTX44Copy        C_MTX44Copy
#define PSMTX44Concat      C_MTX44Concat
#define PSMTX44Transpose   C_MTX44Transpose
#define PSMTX44Trans       C_MTX44Trans
#define PSMTX44TransApply  C_MTX44TransApply
#define PSMTX44Scale       C_MTX44Scale
#define PSMTX44ScaleApply  C_MTX44ScaleApply
#define PSMTX44RotRad      C_MTX44RotRad
#define PSMTX44RotTrig     C_MTX44RotTrig
#define PSMTX44RotAxisRad  C_MTX44RotAxisRad
#define MTX44Identity      C_MTX44Identity
#define MTX44Copy          C_MTX44Copy
#define MTX44Concat        C_MTX44Concat
#define MTX44Transpose     C_MTX44Transpose
#define MTX44Trans         C_MTX44Trans
#define MTX44TransApply    C_MTX44TransApply
#define MTX44Scale         C_MTX44Scale
#define MTX44ScaleApply    C_MTX44ScaleApply
#define MTX44RotRad        C_MTX44RotRad
#define MTX44RotTrig       C_MTX44RotTrig
#define MTX44RotAxisRad    C_MTX44RotAxisRad
#define MTXFrustum         C_MTXFrustum
#define MTXPerspective     C_MTXPerspective
#define MTXOrtho           C_MTXOrtho
#define MTX44Inverse       C_MTX44Inverse

// -- Mtx44 vec ---
void C_MTX44MultVec(const Mtx44 m, const Vec* src, Vec* dst);
void C_MTX44MultVecArray(const Mtx44 m, const Vec* srcBase, Vec* dstBase, u32 count);
void C_MTX44MultVecSR(const Mtx44 m, const Vec* src, Vec* dst);
void C_MTX44MultVecArraySR(const Mtx44 m, const Vec* srcBase, Vec* dstBase, u32 count);
#define PSMTX44MultVec        C_MTX44MultVec
#define PSMTX44MultVecArray   C_MTX44MultVecArray
#define PSMTX44MultVecSR      C_MTX44MultVecSR
#define PSMTX44MultVecArraySR C_MTX44MultVecArraySR
#define MTX44MultVec          C_MTX44MultVec
#define MTX44MultVecArray     C_MTX44MultVecArray
#define MTX44MultVecSR        C_MTX44MultVecSR
#define MTX44MultVecArraySR   C_MTX44MultVecArraySR

// -- ROMtx ---
void PSMTXReorder(const Mtx src, ROMtx dest);
void PSMTXROMultVecArray(const ROMtx m, const Vec* srcBase, Vec* dstBase, u32 count);
void PSMTXROSkin2VecArray(const ROMtx m0, const ROMtx m1, const f32* wtBase, const Vec* srcBase, Vec* dstBase, u32 count);
void PSMTXROMultS16VecArray(const Mtx m, const S16Vec* srcBase, Vec* dstBase, u32 count);
void PSMTXMultS16VecArray(const ROMtx* m, const S16Vec* srcBase, Vec* dstBase, u32 count);

// -- MTXStack ---
void    MTXInitStack(MTXStack* sPtr, u32 numMtx);
MtxPtr  MTXPush(MTXStack* sPtr, const Mtx m);
MtxPtr  MTXPushFwd(MTXStack* sPtr, const Mtx m);
MtxPtr  MTXPushInv(MTXStack* sPtr, const Mtx m);
MtxPtr  MTXPushInvXpose(MTXStack* sPtr, const Mtx m);
MtxPtr  MTXPop(MTXStack* sPtr);
MtxPtr  MTXGetStackPtr(const MTXStack* sPtr);

// -- Vec ---
void C_VECAdd(const Vec* a, const Vec* b, Vec* ab);
void C_VECSubtract(const Vec* a, const Vec* b, Vec* a_b);
void C_VECScale(const Vec* src, Vec* dst, f32 scale);
void C_VECNormalize(const Vec* src, Vec* unit);
f32  C_VECSquareMag(const Vec* v);
f32  C_VECMag(const Vec* v);
f32  C_VECDotProduct(const Vec* a, const Vec* b);
void C_VECCrossProduct(const Vec* a, const Vec* b, Vec* axb);
f32  C_VECSquareDistance(const Vec* a, const Vec* b);
f32  C_VECDistance(const Vec* a, const Vec* b);
void C_VECHalfAngle(const Vec* a, const Vec* b, Vec* half);
void C_VECReflect(const Vec* src, const Vec* normal, Vec* dst);

#define PSVECAdd            C_VECAdd
#define PSVECSubtract       C_VECSubtract
#define PSVECScale          C_VECScale
#define PSVECNormalize      C_VECNormalize
#define PSVECSquareMag      C_VECSquareMag
#define PSVECMag            C_VECMag
#define PSVECDotProduct     C_VECDotProduct
#define PSVECCrossProduct   C_VECCrossProduct
#define PSVECSquareDistance C_VECSquareDistance
#define PSVECDistance       C_VECDistance
#define VECAdd              C_VECAdd
#define VECSubtract         C_VECSubtract
#define VECScale            C_VECScale
#define VECNormalize        C_VECNormalize
#define VECSquareMag        C_VECSquareMag
#define VECMag              C_VECMag
#define VECDotProduct       C_VECDotProduct
#define VECCrossProduct     C_VECCrossProduct
#define VECSquareDistance   C_VECSquareDistance
#define VECDistance         C_VECDistance
#define VECHalfAngle        C_VECHalfAngle
#define VECReflect          C_VECReflect

// -- Quaternion ---
void C_QUATAdd(const Quaternion* p, const Quaternion* q, Quaternion* r);
void C_QUATSubtract(const Quaternion* p, const Quaternion* q, Quaternion* r);
void C_QUATMultiply(const Quaternion* p, const Quaternion* q, Quaternion* pq);
void C_QUATScale(const Quaternion* q, Quaternion* r, f32 scale);
f32  C_QUATDotProduct(const Quaternion* p, const Quaternion* q);
void C_QUATNormalize(const Quaternion* src, Quaternion* unit);
void C_QUATInverse(const Quaternion* src, Quaternion* inv);
void C_QUATDivide(const Quaternion* p, const Quaternion* q, Quaternion* r);
void C_QUATExp(const Quaternion* q, Quaternion* r);
void C_QUATLogN(const Quaternion* q, Quaternion* r);
void C_QUATMakeClosest(const Quaternion* q, const Quaternion* qto, Quaternion* r);
void C_QUATRotAxisRad(Quaternion* r, const Vec* axis, f32 rad);
void C_QUATMtx(Quaternion* r, const Mtx m);
void C_QUATLerp(const Quaternion* p, const Quaternion* q, Quaternion* r, f32 t);
void C_QUATSlerp(const Quaternion* p, const Quaternion* q, Quaternion* r, f32 t);
void C_QUATSquad(const Quaternion* p, const Quaternion* a, const Quaternion* b, const Quaternion* q, Quaternion* r, f32 t);
void C_QUATCompA(const Quaternion* qprev, const Quaternion* q, const Quaternion* qnext, Quaternion* a);

#define PSQUATAdd        C_QUATAdd
#define PSQUATSubtract   C_QUATSubtract
#define PSQUATMultiply   C_QUATMultiply
#define PSQUATScale      C_QUATScale
#define PSQUATDotProduct C_QUATDotProduct
#define PSQUATNormalize  C_QUATNormalize
#define PSQUATInverse    C_QUATInverse
#define PSQUATDivide     C_QUATDivide
#define QUATAdd          C_QUATAdd
#define QUATSubtract     C_QUATSubtract
#define QUATMultiply     C_QUATMultiply
#define QUATScale        C_QUATScale
#define QUATDotProduct   C_QUATDotProduct
#define QUATNormalize    C_QUATNormalize
#define QUATInverse      C_QUATInverse
#define QUATDivide       C_QUATDivide
#define QUATExp          C_QUATExp
#define QUATLogN         C_QUATLogN
#define QUATMakeClosest  C_QUATMakeClosest
#define QUATRotAxisRad   C_QUATRotAxisRad
#define QUATMtx          C_QUATMtx
#define QUATLerp         C_QUATLerp
#define QUATSlerp        C_QUATSlerp
#define QUATSquad        C_QUATSquad
#define QUATCompA        C_QUATCompA

#ifdef __cplusplus
}
#endif
