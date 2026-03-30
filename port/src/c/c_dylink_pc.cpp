#include "c/c_dylink.h"

#include "SSystem/SComponent/c_phase.h"
#include "f_pc/f_pc_name.h"
#include "f_op/f_op_scene_mng.h"
#include "f_op/f_op_scene_req.h"
#include "port/port.h"

namespace {
bool sPortDylInitDone = false;
bool sPortLogoSceneRequested = false;
}

int cDyl_InitAsyncIsDone() {
    return sPortDylInitDone ? 1 : 0;
}

void cDyl_InitAsync() {
    if (!sPortLogoSceneRequested) {
        fpc_ProcID req_id =
            fopScnRq_Request(0, NULL, fpcNm_LOGO_SCENE_e, NULL, 0x7FFF, 0);
        tp::log::info("cDyl_InitAsync[PC]: requesting LOGO_SCENE req_id=%u", req_id);
        sPortLogoSceneRequested = true;
    }
    sPortDylInitDone = true;
}

BOOL cDyl_IsLinked(s16) {
    return TRUE;
}

BOOL cDyl_Unlink(s16) {
    return TRUE;
}

int cDyl_LinkASync(s16) {
    return cPhs_COMPLEATE_e;
}

BOOL cCc_Check() {
    return TRUE;
}

int cDylPhs::phase_01(void*) {
    return cPhs_COMPLEATE_e;
}

int cDylPhs::phase_02(s16*) {
    return cPhs_COMPLEATE_e;
}

int cDylPhs::phase_03(void*) {
    return cPhs_COMPLEATE_e;
}

int cDylPhs::Link(request_of_phase_process_class*, s16) {
    return cPhs_COMPLEATE_e;
}

int cDylPhs::Unlink(request_of_phase_process_class*, s16) {
    return cPhs_COMPLEATE_e;
}
