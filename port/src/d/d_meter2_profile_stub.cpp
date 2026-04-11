#include "d/d_com_inf_game.h"
#include "d/d_meter2.h"
#include "d/d_meter2_info.h"
#include "f_op/f_op_msg.h"
#include "f_op/f_op_msg_mng.h"
#include "f_pc/f_pc_draw_priority.h"
#include "f_pc/f_pc_layer.h"
#include "f_pc/f_pc_leaf.h"

namespace {

struct PortMeter2 : public dMeter2_c {};

static int PortMeter2_Create(PortMeter2* i_this) {
    dMeter2Info_setMeterClass(i_this);
    dComIfGp_2dShowOn();
    fopMsgM_setStageLayer(i_this);
    return cPhs_COMPLEATE_e;
}

static int PortMeter2_Delete(PortMeter2* i_this) {
    if (dMeter2Info_getMeterClass() == i_this) {
        dMeter2Info_setMeterClass(nullptr);
    }
    return 1;
}

static int PortMeter2_Execute(PortMeter2* /*i_this*/) {
    return 1;
}

static int PortMeter2_Draw(PortMeter2* /*i_this*/) {
    return 1;
}

static leafdraw_method_class l_portMeter2Method = {
    (process_method_func)PortMeter2_Create,
    (process_method_func)PortMeter2_Delete,
    (process_method_func)PortMeter2_Execute,
    (process_method_func)nullptr,
    (process_method_func)PortMeter2_Draw,
};

}  // namespace

msg_process_profile_definition g_profile_METER2 = {
    /* Layer ID    */ fpcLy_CURRENT_e,
    /* List ID     */ 12,
    /* List Prio   */ fpcPi_CURRENT_e,
    /* Proc Name   */ fpcNm_METER2_e,
    /* Proc SubMtd */ &g_fpcLf_Method.base,
    /* Size        */ sizeof(PortMeter2),
    /* Size Other  */ 0,
    /* Parameters  */ 0,
    /* Leaf SubMtd */ &g_fopMsg_Method,
    /* Draw Prio   */ fpcDwPi_METER2_e,
    /* Msg SubMtd  */ &l_portMeter2Method,
};
