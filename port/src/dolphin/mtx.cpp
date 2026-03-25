/**
 * src/dolphin/mtx.cpp
 * GameCube MTX matrix/vector/quaternion library — standard C++ implementation.
 *
 * The GC used PPC paired-singles (PS) for SIMD acceleration.
 * On PC we use straightforward scalar math; the compiler will auto-vectorise.
 */

#include "port/port.h"
#include "dolphin/mtx.h"

#include <cmath>
#include <cstring>
#include <cstdlib>

// -----------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------
static inline f32 Dot3(f32 ax, f32 ay, f32 az,
                        f32 bx, f32 by, f32 bz) {
    return ax*bx + ay*by + az*bz;
}

// -----------------------------------------------------------------------
// Mtx (3×4)
// -----------------------------------------------------------------------
void C_MTXIdentity(Mtx m) {
    m[0][0]=1; m[0][1]=0; m[0][2]=0; m[0][3]=0;
    m[1][0]=0; m[1][1]=1; m[1][2]=0; m[1][3]=0;
    m[2][0]=0; m[2][1]=0; m[2][2]=1; m[2][3]=0;
}

void C_MTXCopy(const Mtx src, Mtx dst) {
    memcpy(dst, src, sizeof(Mtx));
}

void C_MTXConcat(const Mtx a, const Mtx b, Mtx ab) {
    Mtx tmp;
    for (int i = 0; i < 3; i++) {
        tmp[i][0] = a[i][0]*b[0][0] + a[i][1]*b[1][0] + a[i][2]*b[2][0];
        tmp[i][1] = a[i][0]*b[0][1] + a[i][1]*b[1][1] + a[i][2]*b[2][1];
        tmp[i][2] = a[i][0]*b[0][2] + a[i][1]*b[1][2] + a[i][2]*b[2][2];
        tmp[i][3] = a[i][0]*b[0][3] + a[i][1]*b[1][3] + a[i][2]*b[2][3] + a[i][3];
    }
    memcpy(ab, tmp, sizeof(Mtx));
}

void C_MTXConcatArray(const Mtx a, const Mtx* srcBase, Mtx* dstBase, u32 count) {
    for (u32 i = 0; i < count; i++)
        C_MTXConcat(a, srcBase[i], dstBase[i]);
}

void C_MTXTranspose(const Mtx src, Mtx xPose) {
    // src is 3×4; transpose the 3×3 rotation, zero the translation
    xPose[0][0]=src[0][0]; xPose[0][1]=src[1][0]; xPose[0][2]=src[2][0]; xPose[0][3]=0;
    xPose[1][0]=src[0][1]; xPose[1][1]=src[1][1]; xPose[1][2]=src[2][1]; xPose[1][3]=0;
    xPose[2][0]=src[0][2]; xPose[2][1]=src[1][2]; xPose[2][2]=src[2][2]; xPose[2][3]=0;
}

u32 C_MTXInverse(const Mtx src, Mtx inv) {
    // Full 3×4 affine inverse
    f32 det = src[0][0]*(src[1][1]*src[2][2]-src[1][2]*src[2][1])
            - src[0][1]*(src[1][0]*src[2][2]-src[1][2]*src[2][0])
            + src[0][2]*(src[1][0]*src[2][1]-src[1][1]*src[2][0]);
    if (fabsf(det) < 1e-10f) return 1; // singular
    f32 rd = 1.0f / det;
    inv[0][0] = rd*(src[1][1]*src[2][2]-src[1][2]*src[2][1]);
    inv[0][1] = rd*(src[0][2]*src[2][1]-src[0][1]*src[2][2]);
    inv[0][2] = rd*(src[0][1]*src[1][2]-src[0][2]*src[1][1]);
    inv[1][0] = rd*(src[1][2]*src[2][0]-src[1][0]*src[2][2]);
    inv[1][1] = rd*(src[0][0]*src[2][2]-src[0][2]*src[2][0]);
    inv[1][2] = rd*(src[0][2]*src[1][0]-src[0][0]*src[1][2]);
    inv[2][0] = rd*(src[1][0]*src[2][1]-src[1][1]*src[2][0]);
    inv[2][1] = rd*(src[0][1]*src[2][0]-src[0][0]*src[2][1]);
    inv[2][2] = rd*(src[0][0]*src[1][1]-src[0][1]*src[1][0]);
    inv[0][3] = -(inv[0][0]*src[0][3]+inv[0][1]*src[1][3]+inv[0][2]*src[2][3]);
    inv[1][3] = -(inv[1][0]*src[0][3]+inv[1][1]*src[1][3]+inv[1][2]*src[2][3]);
    inv[2][3] = -(inv[2][0]*src[0][3]+inv[2][1]*src[1][3]+inv[2][2]*src[2][3]);
    return 0;
}

u32 C_MTXInvXpose(const Mtx src, Mtx invX) {
    Mtx inv;
    u32 r = C_MTXInverse(src, inv);
    C_MTXTranspose(inv, invX);
    return r;
}

void C_MTXRotRad(Mtx m, char axis, f32 rad) {
    f32 s = sinf(rad), c = cosf(rad);
    C_MTXIdentity(m);
    switch (axis) {
        case 'x': case 'X': m[1][1]=c; m[1][2]=-s; m[2][1]=s; m[2][2]=c; break;
        case 'y': case 'Y': m[0][0]=c; m[0][2]=s; m[2][0]=-s; m[2][2]=c; break;
        case 'z': case 'Z': m[0][0]=c; m[0][1]=-s; m[1][0]=s; m[1][1]=c; break;
    }
}

void C_MTXRotTrig(Mtx m, char axis, f32 sinA, f32 cosA) {
    C_MTXIdentity(m);
    switch (axis) {
        case 'x': case 'X': m[1][1]=cosA; m[1][2]=-sinA; m[2][1]=sinA; m[2][2]=cosA; break;
        case 'y': case 'Y': m[0][0]=cosA; m[0][2]=sinA; m[2][0]=-sinA; m[2][2]=cosA; break;
        case 'z': case 'Z': m[0][0]=cosA; m[0][1]=-sinA; m[1][0]=sinA; m[1][1]=cosA; break;
    }
}

void C_MTXRotAxisRad(Mtx m, const Vec* axis, f32 rad) {
    f32 s = sinf(rad), c = cosf(rad), t = 1.0f-c;
    f32 x=axis->x, y=axis->y, z=axis->z;
    m[0][0]=t*x*x+c;   m[0][1]=t*x*y-s*z; m[0][2]=t*x*z+s*y; m[0][3]=0;
    m[1][0]=t*x*y+s*z; m[1][1]=t*y*y+c;   m[1][2]=t*y*z-s*x; m[1][3]=0;
    m[2][0]=t*x*z-s*y; m[2][1]=t*y*z+s*x; m[2][2]=t*z*z+c;   m[2][3]=0;
}

void C_MTXTrans(Mtx m, f32 xT, f32 yT, f32 zT) {
    C_MTXIdentity(m);
    m[0][3]=xT; m[1][3]=yT; m[2][3]=zT;
}

void C_MTXTransApply(const Mtx src, Mtx dst, f32 xT, f32 yT, f32 zT) {
    C_MTXCopy(src, dst);
    dst[0][3]+=xT; dst[1][3]+=yT; dst[2][3]+=zT;
}

void C_MTXScale(Mtx m, f32 xS, f32 yS, f32 zS) {
    memset(m, 0, sizeof(Mtx));
    m[0][0]=xS; m[1][1]=yS; m[2][2]=zS;
}

void C_MTXScaleApply(const Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS) {
    for (int j = 0; j < 4; j++) {
        dst[0][j]=src[0][j]*xS;
        dst[1][j]=src[1][j]*yS;
        dst[2][j]=src[2][j]*zS;
    }
}

void C_MTXQuat(Mtx m, const Quaternion* q) {
    f32 x=q->x,y=q->y,z=q->z,w=q->w;
    m[0][0]=1-2*(y*y+z*z); m[0][1]=2*(x*y-w*z); m[0][2]=2*(x*z+w*y); m[0][3]=0;
    m[1][0]=2*(x*y+w*z); m[1][1]=1-2*(x*x+z*z); m[1][2]=2*(y*z-w*x); m[1][3]=0;
    m[2][0]=2*(x*z-w*y); m[2][1]=2*(y*z+w*x); m[2][2]=1-2*(x*x+y*y); m[2][3]=0;
}

void C_MTXReflect(Mtx m, const Vec* p, const Vec* n) {
    f32 nx=n->x,ny=n->y,nz=n->z;
    f32 d = 2.0f*(nx*p->x+ny*p->y+nz*p->z);
    C_MTXIdentity(m);
    m[0][0]-=2*nx*nx; m[0][1]-=2*nx*ny; m[0][2]-=2*nx*nz; m[0][3]=d*nx;
    m[1][0]-=2*ny*nx; m[1][1]-=2*ny*ny; m[1][2]-=2*ny*nz; m[1][3]=d*ny;
    m[2][0]-=2*nz*nx; m[2][1]-=2*nz*ny; m[2][2]-=2*nz*nz; m[2][3]=d*nz;
}

void C_MTXLookAt(Mtx m, const Point3d* camPos, const Vec* camUp, const Point3d* target) {
    Vec fwd = {target->x-camPos->x, target->y-camPos->y, target->z-camPos->z};
    f32 len = sqrtf(fwd.x*fwd.x+fwd.y*fwd.y+fwd.z*fwd.z);
    if (len > 1e-10f) { fwd.x/=len; fwd.y/=len; fwd.z/=len; }
    Vec right;
    right.x = camUp->y*fwd.z - camUp->z*fwd.y;
    right.y = camUp->z*fwd.x - camUp->x*fwd.z;
    right.z = camUp->x*fwd.y - camUp->y*fwd.x;
    len = sqrtf(right.x*right.x+right.y*right.y+right.z*right.z);
    if (len > 1e-10f) { right.x/=len; right.y/=len; right.z/=len; }
    Vec up;
    up.x = fwd.y*right.z-fwd.z*right.y;
    up.y = fwd.z*right.x-fwd.x*right.z;
    up.z = fwd.x*right.y-fwd.y*right.x;
    m[0][0]=right.x; m[0][1]=right.y; m[0][2]=right.z;
    m[0][3]=-(right.x*camPos->x+right.y*camPos->y+right.z*camPos->z);
    m[1][0]=up.x;    m[1][1]=up.y;    m[1][2]=up.z;
    m[1][3]=-(up.x*camPos->x+up.y*camPos->y+up.z*camPos->z);
    m[2][0]=-fwd.x;  m[2][1]=-fwd.y;  m[2][2]=-fwd.z;
    m[2][3]=(fwd.x*camPos->x+fwd.y*camPos->y+fwd.z*camPos->z);
}

void C_MTXLightFrustum(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n,
                        f32 scaleS, f32 scaleT, f32 transS, f32 transT) {
    C_MTXIdentity(m);
    m[0][0]=scaleS*2*n/(r-l); m[0][2]=scaleS*(r+l)/(r-l)+transS;
    m[1][1]=scaleT*2*n/(t-b); m[1][2]=scaleT*(t+b)/(t-b)+transT;
    m[2][2]=-1; m[2][3]=-n;
}
void C_MTXLightPerspective(Mtx m, f32 fovY, f32 aspect, f32 scaleS, f32 scaleT, f32 transS, f32 transT) {
    f32 ht = tanf(fovY*0.5f);
    C_MTXLightFrustum(m, ht, -ht, -ht*aspect, ht*aspect, 1, scaleS, scaleT, transS, transT);
}
void C_MTXLightOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 scaleS, f32 scaleT, f32 transS, f32 transT) {
    C_MTXIdentity(m);
    m[0][0]=scaleS*2/(r-l); m[0][3]=transS-scaleS*(r+l)/(r-l);
    m[1][1]=scaleT*2/(t-b); m[1][3]=transT-scaleT*(t+b)/(t-b);
}

// -----------------------------------------------------------------------
// Mtx × Vec
// -----------------------------------------------------------------------
void C_MTXMultVec(const Mtx m, const Vec* src, Vec* dst) {
    f32 x=src->x, y=src->y, z=src->z;
    dst->x = m[0][0]*x + m[0][1]*y + m[0][2]*z + m[0][3];
    dst->y = m[1][0]*x + m[1][1]*y + m[1][2]*z + m[1][3];
    dst->z = m[2][0]*x + m[2][1]*y + m[2][2]*z + m[2][3];
}

void C_MTXMultVecArray(const Mtx m, const Vec* srcBase, Vec* dstBase, u32 count) {
    for (u32 i = 0; i < count; i++) C_MTXMultVec(m, &srcBase[i], &dstBase[i]);
}

void C_MTXMultVecSR(const Mtx m, const Vec* src, Vec* dst) {
    // Scale+Rotate only (no translation)
    f32 x=src->x, y=src->y, z=src->z;
    dst->x = m[0][0]*x + m[0][1]*y + m[0][2]*z;
    dst->y = m[1][0]*x + m[1][1]*y + m[1][2]*z;
    dst->z = m[2][0]*x + m[2][1]*y + m[2][2]*z;
}

void C_MTXMultVecArraySR(const Mtx m, const Vec* srcBase, Vec* dstBase, u32 count) {
    for (u32 i = 0; i < count; i++) C_MTXMultVecSR(m, &srcBase[i], &dstBase[i]);
}

// -----------------------------------------------------------------------
// Mtx44 (4×4)
// -----------------------------------------------------------------------
void C_MTX44Identity(Mtx44 m) {
    memset(m, 0, sizeof(Mtx44));
    m[0][0]=m[1][1]=m[2][2]=m[3][3]=1;
}

void C_MTX44Copy(const Mtx44 src, Mtx44 dst) { memcpy(dst, src, sizeof(Mtx44)); }

void C_MTX44Concat(const Mtx44 a, const Mtx44 b, Mtx44 ab) {
    Mtx44 tmp;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            tmp[i][j] = a[i][0]*b[0][j]+a[i][1]*b[1][j]+a[i][2]*b[2][j]+a[i][3]*b[3][j];
    memcpy(ab, tmp, sizeof(Mtx44));
}

void C_MTX44Transpose(const Mtx44 src, Mtx44 xPose) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            xPose[j][i] = src[i][j];
}

void C_MTX44Trans(Mtx44 m, f32 xT, f32 yT, f32 zT) {
    C_MTX44Identity(m); m[0][3]=xT; m[1][3]=yT; m[2][3]=zT;
}

void C_MTX44TransApply(const Mtx44 src, Mtx44 dst, f32 xT, f32 yT, f32 zT) {
    C_MTX44Copy(src, dst); dst[0][3]+=xT; dst[1][3]+=yT; dst[2][3]+=zT;
}

void C_MTX44Scale(Mtx44 m, f32 xS, f32 yS, f32 zS) {
    memset(m, 0, sizeof(Mtx44));
    m[0][0]=xS; m[1][1]=yS; m[2][2]=zS; m[3][3]=1;
}

void C_MTX44ScaleApply(const Mtx44 src, Mtx44 dst, f32 xS, f32 yS, f32 zS) {
    for (int j = 0; j < 4; j++) {
        dst[0][j]=src[0][j]*xS; dst[1][j]=src[1][j]*yS; dst[2][j]=src[2][j]*zS; dst[3][j]=src[3][j];
    }
}

void C_MTX44RotRad(Mtx44 m, char axis, f32 rad) {
    Mtx tmp; C_MTXRotRad(tmp, axis, rad);
    C_MTX44Identity(m);
    for (int i = 0; i < 3; i++) for (int j = 0; j < 4; j++) m[i][j]=tmp[i][j];
}

void C_MTX44RotTrig(Mtx44 m, char axis, f32 sinA, f32 cosA) {
    Mtx tmp; C_MTXRotTrig(tmp, axis, sinA, cosA);
    C_MTX44Identity(m);
    for (int i = 0; i < 3; i++) for (int j = 0; j < 4; j++) m[i][j]=tmp[i][j];
}

void C_MTX44RotAxisRad(Mtx44 m, const Vec* axis, f32 rad) {
    Mtx tmp; C_MTXRotAxisRad(tmp, axis, rad);
    C_MTX44Identity(m);
    for (int i = 0; i < 3; i++) for (int j = 0; j < 4; j++) m[i][j]=tmp[i][j];
}

void C_MTXFrustum(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f) {
    memset(m, 0, sizeof(Mtx44));
    m[0][0]=2*n/(r-l); m[0][2]=(r+l)/(r-l);
    m[1][1]=2*n/(t-b); m[1][2]=(t+b)/(t-b);
    m[2][2]=-(f+n)/(f-n); m[2][3]=-2*f*n/(f-n);
    m[3][2]=-1;
}

void C_MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f) {
    f32 ht = n * tanf(fovY * 0.5f);
    C_MTXFrustum(m, ht, -ht, -ht*aspect, ht*aspect, n, f);
}

void C_MTXOrtho(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f) {
    memset(m, 0, sizeof(Mtx44));
    m[0][0]=2/(r-l); m[0][3]=-(r+l)/(r-l);
    m[1][1]=2/(t-b); m[1][3]=-(t+b)/(t-b);
    m[2][2]=-2/(f-n); m[2][3]=-(f+n)/(f-n);
    m[3][3]=1;
}

u32 C_MTX44Inverse(const Mtx44 src, Mtx44 inv) {
    // Cofactor expansion
    f32 m[16]; for(int i=0;i<4;i++) for(int j=0;j<4;j++) m[i*4+j]=src[i][j];
    f32 tmp[16];
    tmp[0]  =  m[5]*m[10]*m[15]-m[5]*m[11]*m[14]-m[9]*m[6]*m[15]+m[9]*m[7]*m[14]+m[13]*m[6]*m[11]-m[13]*m[7]*m[10];
    tmp[4]  = -m[4]*m[10]*m[15]+m[4]*m[11]*m[14]+m[8]*m[6]*m[15]-m[8]*m[7]*m[14]-m[12]*m[6]*m[11]+m[12]*m[7]*m[10];
    tmp[8]  =  m[4]*m[9]*m[15] -m[4]*m[11]*m[13]-m[8]*m[5]*m[15]+m[8]*m[7]*m[13]+m[12]*m[5]*m[11]-m[12]*m[7]*m[9];
    tmp[12] = -m[4]*m[9]*m[14] +m[4]*m[10]*m[13]+m[8]*m[5]*m[14]-m[8]*m[6]*m[13]-m[12]*m[5]*m[10]+m[12]*m[6]*m[9];
    f32 det = m[0]*tmp[0]+m[1]*tmp[4]+m[2]*tmp[8]+m[3]*tmp[12];
    if (fabsf(det) < 1e-10f) return 1;
    f32 rd = 1.0f/det;
    tmp[1]  = (-m[1]*m[10]*m[15]+m[1]*m[11]*m[14]+m[9]*m[2]*m[15]-m[9]*m[3]*m[14]-m[13]*m[2]*m[11]+m[13]*m[3]*m[10])*rd;
    tmp[5]  = ( m[0]*m[10]*m[15]-m[0]*m[11]*m[14]-m[8]*m[2]*m[15]+m[8]*m[3]*m[14]+m[12]*m[2]*m[11]-m[12]*m[3]*m[10])*rd;
    tmp[9]  = (-m[0]*m[9]*m[15] +m[0]*m[11]*m[13]+m[8]*m[1]*m[15]-m[8]*m[3]*m[13]-m[12]*m[1]*m[11]+m[12]*m[3]*m[9] )*rd;
    tmp[13] = ( m[0]*m[9]*m[14] -m[0]*m[10]*m[13]-m[8]*m[1]*m[14]+m[8]*m[2]*m[13]+m[12]*m[1]*m[10]-m[12]*m[2]*m[9] )*rd;
    tmp[2]  = ( m[1]*m[6]*m[15] -m[1]*m[7]*m[14] -m[5]*m[2]*m[15]+m[5]*m[3]*m[14]+m[13]*m[2]*m[7] -m[13]*m[3]*m[6] )*rd;
    tmp[6]  = (-m[0]*m[6]*m[15] +m[0]*m[7]*m[14] +m[4]*m[2]*m[15]-m[4]*m[3]*m[14]-m[12]*m[2]*m[7] +m[12]*m[3]*m[6] )*rd;
    tmp[10] = ( m[0]*m[5]*m[15] -m[0]*m[7]*m[13] -m[4]*m[1]*m[15]+m[4]*m[3]*m[13]+m[12]*m[1]*m[7] -m[12]*m[3]*m[5] )*rd;
    tmp[14] = (-m[0]*m[5]*m[14] +m[0]*m[6]*m[13] +m[4]*m[1]*m[14]-m[4]*m[2]*m[13]-m[12]*m[1]*m[6] +m[12]*m[2]*m[5] )*rd;
    tmp[3]  = (-m[1]*m[6]*m[11] +m[1]*m[7]*m[10] +m[5]*m[2]*m[11]-m[5]*m[3]*m[10]-m[9]*m[2]*m[7]  +m[9]*m[3]*m[6]  )*rd;
    tmp[7]  = ( m[0]*m[6]*m[11] -m[0]*m[7]*m[10] -m[4]*m[2]*m[11]+m[4]*m[3]*m[10]+m[8]*m[2]*m[7]  -m[8]*m[3]*m[6]  )*rd;
    tmp[11] = (-m[0]*m[5]*m[11] +m[0]*m[7]*m[9]  +m[4]*m[1]*m[11]-m[4]*m[3]*m[9] -m[8]*m[1]*m[7]  +m[8]*m[3]*m[5]  )*rd;
    tmp[15] = ( m[0]*m[5]*m[10] -m[0]*m[6]*m[9]  -m[4]*m[1]*m[10]+m[4]*m[2]*m[9] +m[8]*m[1]*m[6]  -m[8]*m[2]*m[5]  )*rd;
    tmp[0] *= rd; tmp[4] *= rd; tmp[8] *= rd; tmp[12] *= rd;
    for (int i=0;i<4;i++) for(int j=0;j<4;j++) inv[i][j]=tmp[i*4+j];
    return 0;
}

void C_MTX44MultVec(const Mtx44 m, const Vec* src, Vec* dst) {
    f32 x=src->x,y=src->y,z=src->z;
    f32 w = m[3][0]*x+m[3][1]*y+m[3][2]*z+m[3][3];
    dst->x = (m[0][0]*x+m[0][1]*y+m[0][2]*z+m[0][3])/w;
    dst->y = (m[1][0]*x+m[1][1]*y+m[1][2]*z+m[1][3])/w;
    dst->z = (m[2][0]*x+m[2][1]*y+m[2][2]*z+m[2][3])/w;
}

void C_MTX44MultVecArray(const Mtx44 m,const Vec* s,Vec* d,u32 n){for(u32 i=0;i<n;i++)C_MTX44MultVec(m,&s[i],&d[i]);}
void C_MTX44MultVecSR(const Mtx44 m,const Vec* s,Vec* d){d->x=m[0][0]*s->x+m[0][1]*s->y+m[0][2]*s->z;d->y=m[1][0]*s->x+m[1][1]*s->y+m[1][2]*s->z;d->z=m[2][0]*s->x+m[2][1]*s->y+m[2][2]*s->z;}
void C_MTX44MultVecArraySR(const Mtx44 m,const Vec* s,Vec* d,u32 n){for(u32 i=0;i<n;i++)C_MTX44MultVecSR(m,&s[i],&d[i]);}

// -----------------------------------------------------------------------
// ROMtx
// -----------------------------------------------------------------------
void PSMTXReorder(const Mtx src, ROMtx dest) {
    for (int i=0;i<3;i++) for (int j=0;j<4;j++) dest[j][i]=src[i][j];
}
void PSMTXROMultVecArray(const ROMtx m, const Vec* srcBase, Vec* dstBase, u32 count) {
    for (u32 k=0; k<count; k++) {
        f32 x=srcBase[k].x,y=srcBase[k].y,z=srcBase[k].z;
        dstBase[k].x=m[0][0]*x+m[1][0]*y+m[2][0]*z+m[3][0];
        dstBase[k].y=m[0][1]*x+m[1][1]*y+m[2][1]*z+m[3][1];
        dstBase[k].z=m[0][2]*x+m[1][2]*y+m[2][2]*z+m[3][2];
    }
}
void PSMTXROSkin2VecArray(const ROMtx m0,const ROMtx m1,const f32* wt,const Vec* s,Vec* d,u32 n){
    for(u32 k=0;k<n;k++){
        f32 w0=wt[k],w1=1.0f-w0,x=s[k].x,y=s[k].y,z=s[k].z;
        d[k].x=w0*(m0[0][0]*x+m0[1][0]*y+m0[2][0]*z+m0[3][0])+w1*(m1[0][0]*x+m1[1][0]*y+m1[2][0]*z+m1[3][0]);
        d[k].y=w0*(m0[0][1]*x+m0[1][1]*y+m0[2][1]*z+m0[3][1])+w1*(m1[0][1]*x+m1[1][1]*y+m1[2][1]*z+m1[3][1]);
        d[k].z=w0*(m0[0][2]*x+m0[1][2]*y+m0[2][2]*z+m0[3][2])+w1*(m1[0][2]*x+m1[1][2]*y+m1[2][2]*z+m1[3][2]);
    }
}
void PSMTXROMultS16VecArray(const Mtx m,const S16Vec* s,Vec* d,u32 n){
    for(u32 k=0;k<n;k++){f32 x=s[k].x,y=s[k].y,z=s[k].z;d[k].x=m[0][0]*x+m[0][1]*y+m[0][2]*z+m[0][3];d[k].y=m[1][0]*x+m[1][1]*y+m[1][2]*z+m[1][3];d[k].z=m[2][0]*x+m[2][1]*y+m[2][2]*z+m[2][3];}
}
void PSMTXMultS16VecArray(const ROMtx* m,const S16Vec* s,Vec* d,u32 n){
    for(u32 k=0;k<n;k++){f32 x=s[k].x,y=s[k].y,z=s[k].z;d[k].x=(*m)[0][0]*x+(*m)[1][0]*y+(*m)[2][0]*z+(*m)[3][0];d[k].y=(*m)[0][1]*x+(*m)[1][1]*y+(*m)[2][1]*z+(*m)[3][1];d[k].z=(*m)[0][2]*x+(*m)[1][2]*y+(*m)[2][2]*z+(*m)[3][2];}
}

// -----------------------------------------------------------------------
// MTXStack
// -----------------------------------------------------------------------
void MTXInitStack(MTXStack* s, u32 n){ s->numMtx=n; s->stackPtr=s->stackBase; }
MtxPtr MTXPush(MTXStack* s, const Mtx m){ C_MTXConcat(m,(const float(*)[4])s->stackPtr[-1],s->stackPtr); return s->stackPtr++; }
MtxPtr MTXPushFwd(MTXStack* s, const Mtx m){ C_MTXCopy(m,s->stackPtr); return s->stackPtr++; }
MtxPtr MTXPushInv(MTXStack* s, const Mtx m){ C_MTXInverse(m,s->stackPtr); return s->stackPtr++; }
MtxPtr MTXPushInvXpose(MTXStack* s, const Mtx m){ C_MTXInvXpose(m,s->stackPtr); return s->stackPtr++; }
MtxPtr MTXPop(MTXStack* s){ return --s->stackPtr; }
MtxPtr MTXGetStackPtr(const MTXStack* s){ return s->stackPtr; }

// -----------------------------------------------------------------------
// Vec
// -----------------------------------------------------------------------
void C_VECAdd(const Vec* a, const Vec* b, Vec* r){ r->x=a->x+b->x; r->y=a->y+b->y; r->z=a->z+b->z; }
void C_VECSubtract(const Vec* a, const Vec* b, Vec* r){ r->x=a->x-b->x; r->y=a->y-b->y; r->z=a->z-b->z; }
void C_VECScale(const Vec* s, Vec* d, f32 sc){ d->x=s->x*sc; d->y=s->y*sc; d->z=s->z*sc; }
f32  C_VECSquareMag(const Vec* v){ return v->x*v->x+v->y*v->y+v->z*v->z; }
f32  C_VECMag(const Vec* v){ return sqrtf(C_VECSquareMag(v)); }
void C_VECNormalize(const Vec* s, Vec* d){ f32 m=C_VECMag(s); if(m>1e-10f){d->x=s->x/m;d->y=s->y/m;d->z=s->z/m;} }
f32  C_VECDotProduct(const Vec* a, const Vec* b){ return a->x*b->x+a->y*b->y+a->z*b->z; }
void C_VECCrossProduct(const Vec* a, const Vec* b, Vec* r){ r->x=a->y*b->z-a->z*b->y; r->y=a->z*b->x-a->x*b->z; r->z=a->x*b->y-a->y*b->x; }
f32  C_VECSquareDistance(const Vec* a, const Vec* b){ Vec d={a->x-b->x,a->y-b->y,a->z-b->z}; return C_VECSquareMag(&d); }
f32  C_VECDistance(const Vec* a, const Vec* b){ return sqrtf(C_VECSquareDistance(a,b)); }
void C_VECHalfAngle(const Vec* a, const Vec* b, Vec* h){ C_VECAdd(a,b,h); C_VECNormalize(h,h); }
void C_VECReflect(const Vec* s, const Vec* n, Vec* d){ f32 dot=2*C_VECDotProduct(s,n); d->x=s->x-dot*n->x; d->y=s->y-dot*n->y; d->z=s->z-dot*n->z; }

// -----------------------------------------------------------------------
// Quaternion
// -----------------------------------------------------------------------
void C_QUATAdd(const Quaternion* p, const Quaternion* q, Quaternion* r){ r->x=p->x+q->x;r->y=p->y+q->y;r->z=p->z+q->z;r->w=p->w+q->w; }
void C_QUATSubtract(const Quaternion* p, const Quaternion* q, Quaternion* r){ r->x=p->x-q->x;r->y=p->y-q->y;r->z=p->z-q->z;r->w=p->w-q->w; }
void C_QUATMultiply(const Quaternion* p, const Quaternion* q, Quaternion* r){
    r->x=p->w*q->x+p->x*q->w+p->y*q->z-p->z*q->y;
    r->y=p->w*q->y-p->x*q->z+p->y*q->w+p->z*q->x;
    r->z=p->w*q->z+p->x*q->y-p->y*q->x+p->z*q->w;
    r->w=p->w*q->w-p->x*q->x-p->y*q->y-p->z*q->z;
}
void C_QUATScale(const Quaternion* q, Quaternion* r, f32 s){ r->x=q->x*s;r->y=q->y*s;r->z=q->z*s;r->w=q->w*s; }
f32  C_QUATDotProduct(const Quaternion* p, const Quaternion* q){ return p->x*q->x+p->y*q->y+p->z*q->z+p->w*q->w; }
void C_QUATNormalize(const Quaternion* s, Quaternion* d){ f32 m=sqrtf(C_QUATDotProduct(s,s)); if(m>1e-10f){d->x=s->x/m;d->y=s->y/m;d->z=s->z/m;d->w=s->w/m;} }
void C_QUATInverse(const Quaternion* s, Quaternion* d){ d->x=-s->x;d->y=-s->y;d->z=-s->z;d->w=s->w; }
void C_QUATDivide(const Quaternion* p, const Quaternion* q, Quaternion* r){ Quaternion qi; C_QUATInverse(q,&qi); C_QUATMultiply(p,&qi,r); }
void C_QUATExp(const Quaternion* q, Quaternion* r){ f32 a=sqrtf(q->x*q->x+q->y*q->y+q->z*q->z),sa=sinf(a),ea=expf(q->w);f32 c=a>1e-10f?ea*sa/a:0;r->x=c*q->x;r->y=c*q->y;r->z=c*q->z;r->w=ea*cosf(a); }
void C_QUATLogN(const Quaternion* q, Quaternion* r){ f32 v=sqrtf(q->x*q->x+q->y*q->y+q->z*q->z),c=v>1e-10f?atan2f(v,q->w)/v:0;r->x=c*q->x;r->y=c*q->y;r->z=c*q->z;r->w=0.5f*logf(C_QUATDotProduct(q,q)); }
void C_QUATMakeClosest(const Quaternion* q, const Quaternion* qto, Quaternion* r){ if(C_QUATDotProduct(q,qto)<0){r->x=-q->x;r->y=-q->y;r->z=-q->z;r->w=-q->w;}else{*r=*q;} }
void C_QUATRotAxisRad(Quaternion* r, const Vec* ax, f32 rad){ f32 s=sinf(rad*0.5f); r->x=ax->x*s;r->y=ax->y*s;r->z=ax->z*s;r->w=cosf(rad*0.5f); }
void C_QUATMtx(Quaternion* r, const Mtx m){ f32 t=m[0][0]+m[1][1]+m[2][2];if(t>0){f32 s=sqrtf(t+1)*2;r->w=0.25f*s;r->x=(m[2][1]-m[1][2])/s;r->y=(m[0][2]-m[2][0])/s;r->z=(m[1][0]-m[0][1])/s;}else if(m[0][0]>m[1][1]&&m[0][0]>m[2][2]){f32 s=sqrtf(1+m[0][0]-m[1][1]-m[2][2])*2;r->w=(m[2][1]-m[1][2])/s;r->x=0.25f*s;r->y=(m[0][1]+m[1][0])/s;r->z=(m[0][2]+m[2][0])/s;}else if(m[1][1]>m[2][2]){f32 s=sqrtf(1+m[1][1]-m[0][0]-m[2][2])*2;r->w=(m[0][2]-m[2][0])/s;r->x=(m[0][1]+m[1][0])/s;r->y=0.25f*s;r->z=(m[1][2]+m[2][1])/s;}else{f32 s=sqrtf(1+m[2][2]-m[0][0]-m[1][1])*2;r->w=(m[1][0]-m[0][1])/s;r->x=(m[0][2]+m[2][0])/s;r->y=(m[1][2]+m[2][1])/s;r->z=0.25f*s;} }
void C_QUATLerp(const Quaternion* p, const Quaternion* q, Quaternion* r, f32 t){ f32 s=1-t; r->x=s*p->x+t*q->x;r->y=s*p->y+t*q->y;r->z=s*p->z+t*q->z;r->w=s*p->w+t*q->w; C_QUATNormalize(r,r); }
void C_QUATSlerp(const Quaternion* p, const Quaternion* q, Quaternion* r, f32 t){
    f32 dot=C_QUATDotProduct(p,q); Quaternion qq=*q;
    if(dot<0){dot=-dot;qq.x=-qq.x;qq.y=-qq.y;qq.z=-qq.z;qq.w=-qq.w;}
    if(dot>0.9995f){C_QUATLerp(p,&qq,r,t);return;}
    f32 theta=acosf(dot),s=sinf(theta);
    f32 a=sinf((1-t)*theta)/s, b=sinf(t*theta)/s;
    r->x=a*p->x+b*qq.x;r->y=a*p->y+b*qq.y;r->z=a*p->z+b*qq.z;r->w=a*p->w+b*qq.w;
}
void C_QUATSquad(const Quaternion* p,const Quaternion* a,const Quaternion* b,const Quaternion* q,Quaternion* r,f32 t){
    Quaternion tmp1,tmp2; C_QUATSlerp(p,q,&tmp1,t); C_QUATSlerp(a,b,&tmp2,t); C_QUATSlerp(&tmp1,&tmp2,r,2*t*(1-t));
}
void C_QUATCompA(const Quaternion* qp,const Quaternion* q,const Quaternion* qn,Quaternion* a){
    Quaternion qi,l1,l2,s; C_QUATInverse(q,&qi);
    C_QUATMultiply(&qi,qp,&l1); C_QUATLogN(&l1,&l1);
    C_QUATMultiply(&qi,qn,&l2); C_QUATLogN(&l2,&l2);
    C_QUATAdd(&l1,&l2,&s); C_QUATScale(&s,&s,-0.25f);
    C_QUATExp(&s,&s); C_QUATMultiply(q,&s,a);
}
