#include "f_op/f_op_overlap.h"

#include "f_op/f_op_overlap_mng.h"

namespace {

struct PortOverlapTask : public overlap_task_class {};

static int PortOverlap_Create(PortOverlapTask* i_this) {
    fopOvlpM_Done(i_this);
    return 1;
}

static int PortOverlap_Delete(PortOverlapTask* /*i_this*/) {
    return 1;
}

static int PortOverlap_Execute(PortOverlapTask* i_this) {
    if (fopOvlpM_IsOutReq(i_this)) {
        fopOvlpM_Done(i_this);
    }
    return 1;
}

static int PortOverlap_IsDelete(PortOverlapTask* /*i_this*/) {
    return 1;
}

static int PortOverlap_Draw(PortOverlapTask* /*i_this*/) {
    return 1;
}

static leafdraw_method_class l_portOverlapMethod = {
    (process_method_func)PortOverlap_Create,
    (process_method_func)PortOverlap_Delete,
    (process_method_func)PortOverlap_Execute,
    (process_method_func)PortOverlap_IsDelete,
    (process_method_func)PortOverlap_Draw,
};

#define TP_PORT_DEFINE_OVERLAP_PROFILE(symbol, proc_name, draw_prio)                              \
    overlap_process_profile_definition symbol = {                                                 \
        /* Layer ID    */ fpcLy_ROOT_e,                                                           \
        /* List ID     */ 0,                                                                      \
        /* List Prio   */ fpcPi_CURRENT_e,                                                        \
        /* Proc Name   */ proc_name,                                                              \
        /* Proc SubMtd */ &g_fpcLf_Method.base,                                                   \
        /* Size        */ sizeof(PortOverlapTask),                                                \
        /* Size Other  */ 0,                                                                      \
        /* Parameters  */ 0,                                                                      \
        /* Leaf SubMtd */ &g_fopOvlp_Method,                                                      \
        /* Draw Prio   */ draw_prio,                                                              \
        /* Fade SubMtd */ &l_portOverlapMethod,                                                   \
    }

}  // namespace

TP_PORT_DEFINE_OVERLAP_PROFILE(g_profile_OVERLAP0, fpcNm_OVERLAP0_e, fpcDwPi_OVERLAP0_e);
TP_PORT_DEFINE_OVERLAP_PROFILE(g_profile_OVERLAP1, fpcNm_OVERLAP1_e, fpcDwPi_OVERLAP1_e);
TP_PORT_DEFINE_OVERLAP_PROFILE(g_profile_OVERLAP2, fpcNm_OVERLAP2_e, fpcDwPi_OVERLAP2_e);
TP_PORT_DEFINE_OVERLAP_PROFILE(g_profile_OVERLAP3, fpcNm_OVERLAP3_e, fpcDwPi_OVERLAP3_e);
TP_PORT_DEFINE_OVERLAP_PROFILE(g_profile_OVERLAP6, fpcNm_OVERLAP6_e, fpcDwPi_OVERLAP6_e);
TP_PORT_DEFINE_OVERLAP_PROFILE(g_profile_OVERLAP7, fpcNm_OVERLAP7_e, fpcDwPi_OVERLAP7_e);
TP_PORT_DEFINE_OVERLAP_PROFILE(g_profile_OVERLAP8, fpcNm_OVERLAP8_e, fpcDwPi_OVERLAP8_e);
TP_PORT_DEFINE_OVERLAP_PROFILE(g_profile_OVERLAP9, fpcNm_OVERLAP9_e, fpcDwPi_OVERLAP9_e);
TP_PORT_DEFINE_OVERLAP_PROFILE(g_profile_OVERLAP10, fpcNm_OVERLAP10_e, fpcDwPi_OVERLAP10_e);
TP_PORT_DEFINE_OVERLAP_PROFILE(g_profile_OVERLAP11, fpcNm_OVERLAP11_e, fpcDwPi_OVERLAP11_e);

