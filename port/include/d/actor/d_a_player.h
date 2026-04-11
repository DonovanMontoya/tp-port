#pragma once
/**
 * port/include/d/actor/d_a_player.h
 * PC port shim for player declarations.
 *
 * Broad host builds use a lightweight player stub. When compiling the real
 * ALINK/player path, TP_PORT_USE_REAL_ALINK forwards this shim to the decomp
 * header so the full `daPy_*` support types become visible.
 */

#if TP_PORT_USE_REAL_ALINK
#include "../../../../include/d/d_bg_s_acch.h"
#include "../../../../include/d/d_cc_d.h"
#include "../../../../include/d/actor/d_a_player.h"
#else

#include "port/types.h"
#include "Z2AudioLib/Z2SoundObject.h"
#include "f_op/f_op_actor.h"

class daPy_py_c;
class daMidna_c;

class daPy_actorKeep_c {
public:
    daPy_actorKeep_c() = default;
    void setActor() {}
    void setData(fopAc_ac_c* i_actor) { mActor = i_actor; }
    void clearData() {
        mID = (fpc_ProcID)0xFFFFFFFF;
        mActor = nullptr;
    }
    fpc_ProcID getID() const { return mID; }
    void setID(fpc_ProcID i_id) { mID = i_id; }
    fopAc_ac_c* getActor() const { return mActor; }
    fopAc_ac_c* getActorConst() const { return mActor; }

private:
    fpc_ProcID mID = (fpc_ProcID)0xFFFFFFFF;
    fopAc_ac_c* mActor = nullptr;
};

class daPy_py_c : public fopAc_ac_c {
public:
    static u32 checkNowWolf() { return 0; }
    static int checkNowWolfPowerUp() { return 0; }
    static daMidna_c* getMidnaActor() { return nullptr; }
    static void setMidnaMotionNum(int) {}
    static void setMidnaFaceNum(int) {}
    static bool checkRoomRestartStart() { return false; }
    bool checkWolf() { return false; }
    bool checkHorseRide() { return false; }
    bool checkCanoeRide() const { return false; }
    bool checkCanoeSlider() const { return false; }
    bool checkCopyRodTopUse() const { return false; }
    bool checkMagneBootsOn() const { return false; }
    bool checkMidnaRide() const { return false; }
    bool checkFrontRoll() { return false; }
    bool checkWolfDash() { return false; }
    u32 checkPlayerFly() const { return 0; }
    int getSumouMode() const { return 0; }
    void onFrollCrashFlg(u32, BOOL) {}
    fpc_ProcID getGrabActorID() const { return (fpc_ProcID)0xFFFFFFFF; }
    fpc_ProcID getAtnActorID() const { return (fpc_ProcID)0xFFFFFFFF; }
    struct {
        s16 y = 0;
    } shape_angle;
};

daPy_py_c* tpPort_GetPlayerActorStub();
void tpPort_SetPlayerActorStub(daPy_py_c* actor);

inline daPy_py_c* daPy_getPlayerActorClass() { return tpPort_GetPlayerActorStub(); }
inline daPy_py_c* daPy_getLinkPlayerActorClass() { return tpPort_GetPlayerActorStub(); }
#endif

daPy_py_c* tpPort_GetPlayerActorStub();
void tpPort_SetPlayerActorStub(daPy_py_c* actor);
