#pragma once
/**
 * port/include/d/actor/d_a_alink.h
 * PC port shim for Link actor declarations.
 *
 * By default the port uses a lightweight stub so broad host builds do not
 * pull in the full player implementation. When we want to compile the real
 * ALINK translation unit, TP_PORT_USE_REAL_ALINK switches this shim over to
 * the decomp header without changing include order elsewhere.
 */

#if TP_PORT_USE_REAL_ALINK
#include "../../../../include/d/actor/d_a_alink.h"
#else

#include "d/actor/d_a_player.h"

class daDaLink_c;

class daAlink_c : public daPy_py_c {
public:
    static int getBallModelIdx() { return 0; }
    static int getBallBtkIdx() { return 0; }
    static int getBallBrkIdx() { return 0; }
    bool checkRide() const { return false; }
    fopAc_ac_c* getRideActor() { return nullptr; }
    cXyz* getShadowTalkAtnPos() { return &current.pos; }
    void setLastSceneMode(void*) {}
};

inline daAlink_c* daAlink_getAlinkActorClass() {
    return static_cast<daAlink_c*>(tpPort_GetPlayerActorStub());
}

#endif
