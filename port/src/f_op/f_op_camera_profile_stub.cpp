#include "SSystem/SComponent/c_phase.h"
#include "SSystem/SComponent/c_sxyz.h"
#include "SSystem/SComponent/c_tag.h"
#include "f_op/f_op_camera.h"
#include "f_op/f_op_view.h"
#include "f_pc/f_pc_draw_priority.h"
#include "f_pc/f_pc_layer.h"
#include "f_pc/f_pc_leaf.h"
#include "f_pc/f_pc_name.h"
#include "f_pc/f_pc_priority.h"
#include "port/logging.h"

namespace {

struct PortCameraProcess {
    view_class view;
    create_tag_class create_tag;
    leafdraw_method_class* submethod;
    u8 field_0x228[4];
    s8 prm1;
    s8 prm2;
    s8 prm3;
    s8 field_0x22f;
    csXyz angle;
    int field_0x238;
};

static int PortCamera_Create(PortCameraProcess* i_this) {
    i_this->view.near = 1.0f;
    i_this->view.far = 10000.0f;
    i_this->view.fovy = 45.0f;
    i_this->view.aspect = 16.0f / 9.0f;
    i_this->view.lookat.eye.set(0.0f, 0.0f, 0.0f);
    i_this->view.lookat.center.set(0.0f, 0.0f, -1.0f);
    i_this->view.lookat.up.set(0.0f, 1.0f, 0.0f);
    i_this->view.bank = 0;
    tp::log::info("PortCamera: create proc=%d", i_this->view.base.base.profname);
    return cPhs_COMPLEATE_e;
}

static int PortCamera_Delete(PortCameraProcess* i_this) {
    tp::log::info("PortCamera: delete proc=%d", i_this->view.base.base.profname);
    return 1;
}

static int PortCamera_Execute(PortCameraProcess* /*i_this*/) {
    return 1;
}

static int PortCamera_IsDelete(PortCameraProcess* /*i_this*/) {
    return 0;
}

static int PortCamera_Draw(PortCameraProcess* /*i_this*/) {
    return 1;
}

static leafdraw_method_class l_portCamera_Method = {
    (process_method_func)PortCamera_Create,
    (process_method_func)PortCamera_Delete,
    (process_method_func)PortCamera_Execute,
    (process_method_func)PortCamera_IsDelete,
    (process_method_func)PortCamera_Draw,
};

}  // namespace

camera_process_profile_definition g_profile_CAMERA = {
    /* Layer ID           */ fpcLy_CURRENT_e,
    /* List ID            */ 11,
    /* List Prio          */ fpcPi_CURRENT_e,
    /* Proc Name          */ fpcNm_CAMERA_e,
    /* Proc SubMtd        */ &g_fpcLf_Method.base,
    /* Size               */ sizeof(PortCameraProcess),
    /* Size Other         */ 0,
    /* Parameters         */ 0,
    /* View Leaf SubMtd   */ &g_fopVw_Method,
    /* Draw Prio          */ fpcDwPi_CAMERA_e,
    /* fopCam Leaf SubMtd */ &g_fopCam_Method,
                             0,
                             0,
                             0,
                             0,
                             0,
    /* Camera SubMtd      */ &l_portCamera_Method,
                             0,
};

camera_process_profile_definition g_profile_CAMERA2 = {
    /* Layer ID           */ fpcLy_CURRENT_e,
    /* List ID            */ 11,
    /* List Prio          */ fpcPi_CURRENT_e,
    /* Proc Name          */ fpcNm_CAMERA2_e,
    /* Proc SubMtd        */ &g_fpcLf_Method.base,
    /* Size               */ sizeof(PortCameraProcess),
    /* Size Other         */ 0,
    /* Parameters         */ 0,
    /* View Leaf SubMtd   */ &g_fopVw_Method,
    /* Draw Prio          */ fpcDwPi_CAMERA2_e,
    /* fopCam Leaf SubMtd */ &g_fopCam_Method,
                             0,
                             0,
                             0,
                             0,
                             0,
    /* Camera SubMtd      */ &l_portCamera_Method,
                             0,
};
