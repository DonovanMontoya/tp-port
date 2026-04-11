#pragma once
/**
 * port/include/d/d_particle.h
 * PC port stub — particle control.
 * Shadows the original which pulls in JParticle → gx.h chains.
 * f_pc_manager.cpp only calls isStatus()/offStatus() on dPa_control_c.
 */

#if TP_PORT_USE_REAL_ALINK || TP_PORT_USE_REAL_PARTICLE
#ifndef DEG_TO_RAD
#include <cmath>
#define DEG_TO_RAD(x) ((x) * (static_cast<f32>(M_PI) / 180.0f))
#endif
#include "../../../include/d/d_particle.h"
#else

#include "JSystem/JKernel/JKRHeap.h"
#include "port/types.h"
#include "SSystem/SComponent/c_bg_s_poly_info.h"
#include "SSystem/SComponent/c_sxyz.h"
#include "SSystem/SComponent/c_xyz.h"
#include "d/d_kankyo_tev_str.h"

class JPABaseEmitter;
class JPABaseParticle;
class JPADrawInfo;
class mDoDvdThd_toMainRam_c;
class dPa_levelEcallBack;
class dPa_selectTexEcallBack;
class dPa_setColorEcallBack;
class dPa_wbPcallBack_c;
class dPa_fsenthPcallBack;
class dPa_light8EcallBack;
class dPa_light8PcallBack;
class dPa_gen_b_light8EcallBack;
class dPa_gen_b_light8PcallBack;
class dPa_gen_d_light8EcallBack;
class dPa_gen_d_light8PcallBack;
class dPa_particleTracePcallBack_c;

#ifndef JPAPARTICLE_H
class JPAParticleCallBack {
public:
    virtual ~JPAParticleCallBack() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*) {}
    virtual void draw(JPABaseEmitter*, JPABaseParticle*) {}
};
#endif

#ifndef JPAEMITTER_H
class JPAEmitterCallBack {
public:
    virtual ~JPAEmitterCallBack() {}
    virtual void execute(JPABaseEmitter*) {}
    virtual void executeAfter(JPABaseEmitter*) {}
    virtual void draw(JPABaseEmitter*) {}
    virtual void drawAfter(JPABaseEmitter*) {}
};
#endif

void dPa_cleanupGX();

class dPa_control_c {
public:
    static bool isStatus(int /*flag*/) { return false; }
    static void onStatus(int /*flag*/) {}
    static void offStatus(int /*flag*/) {}
    static void cleanupGX() { dPa_cleanupGX(); }

    JKRExpHeap* getResHeap() { return nullptr; }
    void levelExecute(u32) {}
    void createCommon(const void*) {}
    void readScene(u8, mDoDvdThd_toMainRam_c**) {}
    void createScene(const void*) {}
    void removeScene(bool) {}
    void cleanup() {}
    void calc3D() {}
    void calc2D() {}
    void calcMenu() {}
    void drawNormal(JPADrawInfo*) {}
    void drawNormalFog(JPADrawInfo*) {}
    void drawNormalP1(JPADrawInfo*) {}
    void drawProjection(JPADrawInfo*) {}
    void drawNormalPri0_A(JPADrawInfo*) {}
    void drawNormalPri0_B(JPADrawInfo*) {}
    void drawFogPri0_A(JPADrawInfo*) {}
    void drawFogPri0_B(JPADrawInfo*) {}
    void drawFogPri1(JPADrawInfo*) {}
    void drawFogPri2(JPADrawInfo*) {}
    void drawFogPri3(JPADrawInfo*) {}
    void drawFogPri4(JPADrawInfo*) {}
    void drawDarkworld(JPADrawInfo*) {}
    void drawFogScreen(JPADrawInfo*) {}
    void draw2Dgame(JPADrawInfo*) {}
    void draw2Dfore(JPADrawInfo*) {}
    void draw2Dback(JPADrawInfo*) {}
    void draw2DmenuFore(JPADrawInfo*) {}
    void draw2DmenuBack(JPADrawInfo*) {}
    template <typename... Args>
    void setHitMark(Args...) {}
    template <typename... Args>
    void setWaterRipple(Args...) {}
    u32 setPoly(u32, u16, cBgS_PolyInfo&, const cXyz*, const dKy_tevstr_c*, const csXyz*,
                const cXyz*, int, dPa_levelEcallBack*, s8, const cXyz*) { return 0; }
    JPABaseEmitter* setPoly(u16, cBgS_PolyInfo&, const cXyz*, const dKy_tevstr_c*, const csXyz*,
                            const cXyz*, int, dPa_levelEcallBack*, s8, const cXyz*) {
        return nullptr;
    }
    template <typename... Args>
    void setSimple(Args...) {}
    u32 setStopContinue(u32 id) { return id; }
    u32 setSimpleFoot(u32, u32*, cBgS_PolyInfo&, const cXyz*, const dKy_tevstr_c*, int,
                      const csXyz*, const cXyz*, dPa_levelEcallBack*, s8, const cXyz*) {
        return 0;
    }
    u16 setCommonPoly(u32*, cBgS_PolyInfo*, const cXyz*, const cXyz*, const dKy_tevstr_c*, u32,
                      u32, const csXyz*, const cXyz*, s8) {
        return 0;
    }
    void forceOnEventMove(u32) {}
    void levelAllForceOnEventMove() {}
    JPABaseEmitter* getEmitter(u32) { return nullptr; }
    u32 setNormal(u32, u16, const cXyz*, const dKy_tevstr_c*, const csXyz*, const cXyz*, u8,
                  dPa_levelEcallBack*, s8, const GXColor*, const GXColor*, const cXyz*, f32) {
        return 0;
    }
    JPABaseEmitter* setNormal(u16, const cXyz*, const dKy_tevstr_c*, const csXyz*, const cXyz*,
                              u8, dPa_levelEcallBack*, s8, const GXColor*, const GXColor*,
                              const cXyz*, f32) {
        return nullptr;
    }
    u32 getHeapSize() { return 0; }
    u32 getSceneHeapSize() { return 0; }
    u32 getEmitterNum() { return 0; }
    u32 getParticleNum() { return 0; }
    static dPa_light8PcallBack* getLight8PcallBack() { return &mLight8PcallBack; }
    static dPa_gen_b_light8PcallBack* getGen_b_Light8PcallBack() { return &m_b_Light8PcallBack; }
    static dPa_gen_d_light8PcallBack* getGen_d_Light8PcallBack() { return &m_d_Light8PcallBack; }
    static dPa_gen_b_light8EcallBack* getGen_b_Light8EcallBack() { return &m_b_Light8EcallBack; }
    static dPa_gen_d_light8EcallBack* getGen_d_Light8EcallBack() { return &m_d_Light8EcallBack; }
    static dPa_light8EcallBack* getLight8EcallBack() { return &mLight8EcallBack; }
    static dPa_particleTracePcallBack_c* getParticleTracePCB() { return &mParticleTracePCB; }
    static dPa_fsenthPcallBack* getFsenthPcallBack() { return &mFsenthPcallBack; }
    static dPa_wbPcallBack_c* getWaterBubblePcallBack() { return &mWaterBubblePcallBack; }

    static dPa_selectTexEcallBack mTsubo[8];
    static dPa_setColorEcallBack mLifeBall[3];
    static dPa_wbPcallBack_c mWaterBubblePcallBack;
    static dPa_fsenthPcallBack mFsenthPcallBack;
    static dPa_light8EcallBack mLight8EcallBack;
    static dPa_light8PcallBack mLight8PcallBack;
    static dPa_gen_b_light8EcallBack m_b_Light8EcallBack;
    static dPa_gen_b_light8PcallBack m_b_Light8PcallBack;
    static dPa_gen_d_light8EcallBack m_d_Light8EcallBack;
    static dPa_gen_d_light8PcallBack m_d_Light8PcallBack;
    static dPa_particleTracePcallBack_c mParticleTracePCB;
    static u8 mStatus;
    static Mtx mWindViewMatrix;
};

class dPa_levelEcallBack : public JPAEmitterCallBack {
public:
    virtual ~dPa_levelEcallBack() {}
    virtual void setup(JPABaseEmitter*, const cXyz*, const csXyz*, s8) {}
    virtual void cleanup() {}
};

class dPa_selectTexEcallBack : public dPa_levelEcallBack {
public:
    dPa_selectTexEcallBack(u8 param_0) : field_0x4(param_0) {}
    virtual ~dPa_selectTexEcallBack() {}
    virtual void draw(JPABaseEmitter*);
    virtual void setup(JPABaseEmitter*, cXyz const*, csXyz const*, s8) {}

    u8 field_0x4;
};

class dPa_setColorEcallBack : public dPa_levelEcallBack {
public:
    dPa_setColorEcallBack(const GXColor& color) : mColor(color) {}

    virtual ~dPa_setColorEcallBack() {}
    virtual void draw(JPABaseEmitter* i_emitter) {
        UNUSED(i_emitter);
        GXSetTevColor(GX_TEVREG1, mColor);
    }
    virtual void setup(JPABaseEmitter*, cXyz const*, csXyz const*, s8) {}

    GXColor mColor;
};

class dPa_followEcallBack : public dPa_levelEcallBack {
public:
    dPa_followEcallBack(u8 param_0 = 0, u8 param_1 = 0);
    virtual ~dPa_followEcallBack() {}
    virtual void execute(JPABaseEmitter*);
    virtual void draw(JPABaseEmitter*);
    virtual void setup(JPABaseEmitter*, cXyz const*, csXyz const*, s8);
    virtual void cleanup() { end(); }
    virtual void end();

    void remove() { end(); }
    JPABaseEmitter* getEmitter() { return mpEmitter; }
    int isEnd() { return field_0x10 & 1; }

    JPABaseEmitter* mpEmitter = nullptr;
    const cXyz* field_0x8 = nullptr;
    const csXyz* field_0xc = nullptr;
    u8 field_0x10 = 0;
    u8 field_0x11 = 0;
    u8 field_0x12 = 0;
    u8 field_0x13 = 0;
};

class dPa_hermiteEcallBack_c : public dPa_levelEcallBack {
public:
    virtual ~dPa_hermiteEcallBack_c() {}
};

class dPa_light8PcallBack : public JPAParticleCallBack {
public:
    virtual ~dPa_light8PcallBack() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*) {}
    virtual void draw(JPABaseEmitter*, JPABaseParticle*) {}
};

class dPa_gen_b_light8PcallBack : public JPAParticleCallBack {
public:
    virtual ~dPa_gen_b_light8PcallBack() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
    virtual void draw(JPABaseEmitter*, JPABaseParticle*);
};

class dPa_gen_d_light8PcallBack : public JPAParticleCallBack {
public:
    virtual ~dPa_gen_d_light8PcallBack() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
    virtual void draw(JPABaseEmitter*, JPABaseParticle*);
};

class dPa_wbPcallBack_c : public JPAParticleCallBack {
public:
    virtual ~dPa_wbPcallBack_c() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
};

class dPa_fsenthPcallBack : public JPAParticleCallBack {
public:
    virtual ~dPa_fsenthPcallBack() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
    virtual void draw(JPABaseEmitter*, JPABaseParticle*);
};

class dPa_light8EcallBack : public dPa_levelEcallBack {
public:
    virtual void draw(JPABaseEmitter*);
    virtual void drawAfter(JPABaseEmitter* i_emitter) {
        UNUSED(i_emitter);
        dPa_cleanupGX();
    }
    virtual void setup(JPABaseEmitter*, cXyz const*, csXyz const*, s8);
};

class dPa_gen_b_light8EcallBack : public dPa_levelEcallBack {
public:
    virtual void draw(JPABaseEmitter*);
    virtual void drawAfter(JPABaseEmitter* i_emitter) {
        UNUSED(i_emitter);
        dPa_cleanupGX();
    }
    virtual void setup(JPABaseEmitter*, cXyz const*, csXyz const*, s8);
};

class dPa_gen_d_light8EcallBack : public dPa_levelEcallBack {
public:
    virtual void draw(JPABaseEmitter*);
    virtual void drawAfter(JPABaseEmitter* i_emitter) {
        UNUSED(i_emitter);
        dPa_cleanupGX();
    }
    virtual void setup(JPABaseEmitter*, cXyz const*, csXyz const*, s8);
};

class dPa_particleTracePcallBack_c : public JPAParticleCallBack {
public:
    virtual ~dPa_particleTracePcallBack_c() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
};

void dPa_cleanupGX();

#endif
