#include "port/boot_scene.h"

#include "SSystem/SComponent/c_phase.h"
#include "f_op/f_op_actor_mng.h"
#include "f_pc/f_pc_name.h"
#include "f_pc/f_pc_node.h"
#include "f_op/f_op_scene.h"
#include "port/logging.h"
#include "window/window.h"

#include <glad/glad.h>

namespace {

struct PortBootScene {
    scene_class base;
    u32 frame;
    s16 procName;
    fpc_ProcID titleActorId;
};

static int PortBootScene_CreateCommon(scene_class* i_this, s16 proc_name) {
    auto* scene = reinterpret_cast<PortBootScene*>(i_this);
    scene->frame = 0;
    scene->procName = proc_name;
    scene->titleActorId = fpcM_ERROR_PROCESS_ID_e;
    tp::log::info("PortBootScene: create complete proc=%d", proc_name);
    return cPhs_COMPLEATE_e;
}

static int PortMenuScene_Create(scene_class* i_this) {
    return PortBootScene_CreateCommon(i_this, fpcNm_MENU_SCENE_e);
}

static int PortOpeningScene_Create(scene_class* i_this) {
    return PortBootScene_CreateCommon(i_this, fpcNm_OPENING_SCENE_e);
}

static int PortBootScene_Delete(scene_class* /*i_this*/) {
    tp::log::info("PortBootScene: delete");
    return 1;
}

static int PortBootScene_Execute(scene_class* i_this) {
    auto* scene = reinterpret_cast<PortBootScene*>(i_this);
    ++scene->frame;

    if (scene->procName == fpcNm_OPENING_SCENE_e && scene->titleActorId == fpcM_ERROR_PROCESS_ID_e) {
        scene->titleActorId = fopAcM_create(fpcNm_TITLE_e, 0, NULL, -1, NULL, NULL, -1);
        tp::log::info("PortBootScene: requested TITLE actor id=%d", scene->titleActorId);
    }

    return 1;
}

static int PortBootScene_IsDelete(scene_class* /*i_this*/) {
    return 0;
}

static int PortBootScene_Draw(scene_class* i_this) {
    auto* scene = reinterpret_cast<PortBootScene*>(i_this);

    int width = 0;
    int height = 0;
    tp::window::GetFramebufferSize(&width, &height);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);

    if (scene->procName == fpcNm_OPENING_SCENE_e) {
        const float pulse = 0.5f + 0.5f * ((scene->frame % 180) / 179.0f);
        const int horizon = height / 3;
        const int titleBandH = height / 5;
        const int titleBandY = (height * 11) / 20;
        const int glowW = width / 3;
        const int glowX = (width - glowW) / 2;
        const int moonSize = height / 4;
        const int moonX = (width * 3) / 5;
        const int moonY = (height * 3) / 5;

        glScissor(0, 0, width, height);
        glClearColor(0.05f, 0.07f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor(0, height - horizon, width, horizon + height / 8);
        glClearColor(0.16f, 0.22f, 0.36f + 0.16f * pulse, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor(0, titleBandY, width, titleBandH);
        glClearColor(0.34f, 0.25f, 0.10f + 0.14f * pulse, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor(glowX, titleBandY - titleBandH / 2, glowW, titleBandH * 2);
        glClearColor(0.70f + 0.18f * pulse, 0.54f + 0.12f * pulse, 0.20f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor(moonX, moonY, moonSize, moonSize);
        glClearColor(0.72f, 0.80f, 0.96f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor(0, 0, width, height / 6);
        glClearColor(0.06f, 0.05f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    } else {
        const float pulse = 0.5f + 0.5f * ((scene->frame % 240) / 239.0f);
        const int band0 = width / 3;
        const int band1 = (width * 2) / 3;

        glScissor(0, 0, width, height);
        glClearColor(0.04f, 0.04f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor(0, 0, band0, height);
        glClearColor(0.85f, 0.20f + 0.25f * pulse, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor(band0, 0, band1 - band0, height);
        glClearColor(0.10f, 0.65f, 0.18f + 0.35f * pulse, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor(band1, 0, width - band1, height);
        glClearColor(0.10f + 0.25f * pulse, 0.22f, 0.85f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);

    if (scene->frame <= 3) {
        tp::log::info("PortBootScene: draw frame=%u size=%dx%d proc=%d",
                      scene->frame, width, height, scene->procName);
    }

    return 1;
}

static scene_method_class l_portMenuScene_Method = {
    (process_method_func)PortMenuScene_Create,
    (process_method_func)PortBootScene_Delete,
    (process_method_func)PortBootScene_Execute,
    (process_method_func)PortBootScene_IsDelete,
    (process_method_func)PortBootScene_Draw,
};

static scene_method_class l_portOpeningScene_Method = {
    (process_method_func)PortOpeningScene_Create,
    (process_method_func)PortBootScene_Delete,
    (process_method_func)PortBootScene_Execute,
    (process_method_func)PortBootScene_IsDelete,
    (process_method_func)PortBootScene_Draw,
};

}  // namespace

scene_process_profile_definition g_profile_PORT_MENU_SCENE = {
    /* Layer ID     */ fpcLy_ROOT_e,
    /* List ID      */ 1,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_MENU_SCENE_e,
    /* Proc SubMtd  */ &g_fpcNd_Method.base,
    /* Size         */ sizeof(PortBootScene),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopScn_Method.base,
    /* Scene SubMtd */ &l_portMenuScene_Method,
                       0,
};

scene_process_profile_definition g_profile_PORT_OPENING_SCENE = {
    /* Layer ID     */ fpcLy_ROOT_e,
    /* List ID      */ 1,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_OPENING_SCENE_e,
    /* Proc SubMtd  */ &g_fpcNd_Method.base,
    /* Size         */ sizeof(PortBootScene),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopScn_Method.base,
    /* Scene SubMtd */ &l_portOpeningScene_Method,
                       0,
};
