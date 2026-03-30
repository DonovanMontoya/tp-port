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
    static u32  checkNowWolf()              { return 0; }
    static bool checkRoomRestartStart()     { return false; }
    bool checkWolf()                        { return false; }
    bool checkHorseRide()                   { return false; }
    bool checkFrontRoll()                   { return false; }
    bool checkWolfDash()                    { return false; }
    void onFrollCrashFlg(u32, BOOL)         {}
    fpc_ProcID getGrabActorID() const       { return (fpc_ProcID)0xFFFFFFFF; }
    fpc_ProcID getAtnActorID() const        { return (fpc_ProcID)0xFFFFFFFF; }
    struct {
        s16 y = 0;
    } shape_angle;
};

// Accessor stubs — player actor is never spawned in our minimal boot
inline daPy_py_c* daPy_getPlayerActorClass() { return nullptr; }
inline daPy_py_c* daPy_getLinkPlayerActorClass() { return nullptr; }

#endif /* D_A_D_A_PLAYER_H */
