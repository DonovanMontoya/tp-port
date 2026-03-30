#pragma once
/**
 * port/include/d/d_particle.h
 * PC port stub — particle control.
 * Shadows the original which pulls in JParticle → gx.h chains.
 * f_pc_manager.cpp only calls isStatus()/offStatus() on dPa_control_c.
 */
#ifndef D_PARTICLE_D_PARTICLE_H
#define D_PARTICLE_D_PARTICLE_H

#include "JSystem/JKernel/JKRHeap.h"
#include "port/types.h"

class JPABaseEmitter;
class JPABaseParticle;
class JPADrawInfo;
class mDoDvdThd_toMainRam_c;
class dPa_levelEcallBack;

#ifndef JPAPARTICLE_H
class JPAParticleCallBack {
public:
    virtual ~JPAParticleCallBack() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*) {}
    virtual void draw(JPABaseEmitter*, JPABaseParticle*) {}
};
#endif

class dPa_control_c {
public:
    static bool isStatus(int /*flag*/) { return false; }
    static void onStatus(int /*flag*/) {}
    static void offStatus(int /*flag*/) {}
    static void cleanupGX() {}

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
};

inline void dPa_cleanupGX() {}

#endif /* D_PARTICLE_D_PARTICLE_H */
