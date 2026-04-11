#include "f_op/f_op_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "f_pc/f_pc_draw_priority.h"
#include "f_pc/f_pc_layer.h"
#include "f_pc/f_pc_leaf.h"
#include "f_pc/f_pc_manager.h"
#include "SSystem/SComponent/c_phase.h"
#include "port/logging.h"

namespace {

struct PortMissingActor : public fopAc_ac_c {
    u32 frame;
};

static int PortMissingActor_Create(PortMissingActor* i_this) {
    i_this->frame = 0;
    tp::log::info("PortMissingActor: create proc=%d id=%u room=%d params=%08x pos=(%.1f, %.1f, %.1f)",
                  fpcM_GetName(i_this), i_this->base.base.id, i_this->home.roomNo,
                  fopAcM_GetParam(i_this), i_this->current.pos.x, i_this->current.pos.y,
                  i_this->current.pos.z);
    return cPhs_COMPLEATE_e;
}

static int PortMissingActor_Delete(PortMissingActor* i_this) {
    tp::log::info("PortMissingActor: delete proc=%d id=%u", fpcM_GetName(i_this),
                  i_this->base.base.id);
    return 1;
}

static int PortMissingActor_Execute(PortMissingActor* i_this) {
    ++i_this->frame;
    return 1;
}

static int PortMissingActor_Draw(PortMissingActor* /*i_this*/) {
    return 1;
}

static actor_method_class l_portMissingActorMethod = {
    (process_method_func)PortMissingActor_Create,
    (process_method_func)PortMissingActor_Delete,
    (process_method_func)PortMissingActor_Execute,
    (process_method_func)NULL,
    (process_method_func)PortMissingActor_Draw,
};

static actor_process_profile_definition l_missingActorProfile = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ 0,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(PortMissingActor),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_GRASS_e,
    /* Actor SubMtd */ &l_portMissingActorMethod,
    /* Status       */ fopAcStts_NOPAUSE_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};

}  // namespace

extern "C" process_profile_definition* tpPort_GetMissingActorProfile() {
    return reinterpret_cast<process_profile_definition*>(&l_missingActorProfile);
}
