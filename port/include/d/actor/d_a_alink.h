#pragma once
/**
 * port/include/d/actor/d_a_alink.h
 * PC port stub — Link actor class.
 * daAlink_c is used in fopEn_enemy_c::initBallModel() (non-debug code) to obtain
 * ball model resource indices; all return 0 (no resources on PC).
 */
#ifndef D_A_D_A_ALINK_H
#define D_A_D_A_ALINK_H

class daDaLink_c;

class daAlink_c {
public:
    static int getBallModelIdx() { return 0; }
    static int getBallBtkIdx()   { return 0; }
    static int getBallBrkIdx()   { return 0; }
    void setLastSceneMode(void*) {}
};

inline daAlink_c* daAlink_getAlinkActorClass() { return nullptr; }

#endif /* D_A_D_A_ALINK_H */
