#include "SSystem/SComponent/c_phase.h"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "f_pc/f_pc_draw_priority.h"
#include "f_pc/f_pc_layer.h"
#include "f_pc/f_pc_leaf.h"
#include "f_pc/f_pc_name.h"
#include "f_pc/f_pc_priority.h"
#include "port/logging.h"

namespace {

static daPy_py_c* s_playerActorStub = nullptr;

#if !TP_PORT_USE_REAL_ALINK_PROFILE
struct PortAlinkProcess : public daAlink_c {
    u32 frame;
};

static int PortAlink_Create(PortAlinkProcess* i_this) {
    i_this->frame = 0;
    s_playerActorStub = i_this;

    dComIfGp_setPlayer(0, i_this);
    dComIfGp_setPlayerInfo(0, i_this, 0);
    dComIfGp_setLinkPlayer(i_this);
    fopAcM_setStageLayer(i_this);

    tp::log::info(
        "PortAlink: create id=%u room=%d params=%08x pos=(%.1f, %.1f, %.1f)",
        i_this->base.base.id, i_this->home.roomNo, fopAcM_GetParam(i_this), i_this->current.pos.x,
        i_this->current.pos.y, i_this->current.pos.z);
    return cPhs_COMPLEATE_e;
}

static int PortAlink_Delete(PortAlinkProcess* i_this) {
    if (dComIfGp_getPlayer(0) == i_this) {
        dComIfGp_setPlayer(0, NULL);
        dComIfGp_setPlayerInfo(0, NULL, 0);
    }
    if (dComIfGp_getLinkPlayer() == i_this) {
        dComIfGp_setLinkPlayer(NULL);
    }
    if (s_playerActorStub == i_this) {
        s_playerActorStub = nullptr;
    }
    tp::log::info("PortAlink: delete id=%u", i_this->base.base.id);
    return 1;
}

static int PortAlink_Execute(PortAlinkProcess* i_this) {
    ++i_this->frame;
    if (i_this->frame <= 3) {
        tp::log::info("PortAlink: execute frame=%u pos=(%.1f, %.1f, %.1f)", i_this->frame,
                      i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    }
    return 1;
}

static int PortAlink_Draw(PortAlinkProcess* /*i_this*/) {
    return 1;
}

static actor_method_class l_portAlinkMethod = {
    (process_method_func)PortAlink_Create,
    (process_method_func)PortAlink_Delete,
    (process_method_func)PortAlink_Execute,
    (process_method_func)NULL,
    (process_method_func)PortAlink_Draw,
};
#endif

}  // namespace

daPy_py_c* tpPort_GetPlayerActorStub() {
    return s_playerActorStub;
}

void tpPort_SetPlayerActorStub(daPy_py_c* actor) {
    s_playerActorStub = actor;
}

#if !TP_PORT_USE_REAL_ALINK_PROFILE
actor_process_profile_definition g_profile_ALINK = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 5,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_ALINK_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(PortAlinkProcess),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_ALINK_e,
    /* Actor SubMtd */ &l_portAlinkMethod,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_NOPAUSE_e | fopAcStts_FREEZE_e,
    /* Group        */ fopAc_PLAYER_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
#endif
