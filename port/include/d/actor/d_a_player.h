#pragma once
/**
 * port/include/d/actor/d_a_player.h
 * PC port stub — Link/player actor class.
 * Shadows the original which pulls in d_drawlist.h → J2D chain.
 */
#ifndef D_A_D_A_PLAYER_H
#define D_A_D_A_PLAYER_H

#include "port/types.h"
#include "f_op/f_op_actor.h"

class daPy_py_c : public fopAc_ac_c {
public:
    // Methods used in f_op_actor_mng.cpp (non-debug paths)
    static u32  checkNowWolf()   { return 0; }
    bool checkWolf()             { return false; }
    bool checkHorseRide()        { return false; }
};

// Accessor stubs — player actor is never spawned in our minimal boot
inline daPy_py_c* dComIfGp_getLinkPlayer()   { return nullptr; }
inline daPy_py_c* daPy_getPlayerActorClass() { return nullptr; }
inline daPy_py_c* daPy_getLinkPlayerActorClass() { return nullptr; }

#endif /* D_A_D_A_PLAYER_H */
