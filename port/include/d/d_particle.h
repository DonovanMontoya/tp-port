#pragma once
/**
 * port/include/d/d_particle.h
 * PC port stub — particle control.
 * Shadows the original which pulls in JParticle → gx.h chains.
 * f_pc_manager.cpp only calls isStatus()/offStatus() on dPa_control_c.
 */
#ifndef D_PARTICLE_D_PARTICLE_H
#define D_PARTICLE_D_PARTICLE_H

#include "port/types.h"

struct dPa_control_c {
    static bool isStatus(int /*flag*/) { return false; }
    static void offStatus(int /*flag*/) {}
    static void cleanupGX() {}
};

inline void dPa_cleanupGX() {}

#endif /* D_PARTICLE_D_PARTICLE_H */
