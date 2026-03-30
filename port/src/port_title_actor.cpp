#if !defined(TP_PORT_USE_REAL_TITLE) || !TP_PORT_USE_REAL_TITLE

#include "f_op/f_op_actor_mng.h"

#include "JSystem/JUtility/JUTGamePad.h"
#include "dolphin/pad.h"
#include "f_pc/f_pc_name.h"
#include "port/logging.h"
#include "window/window.h"

#include <glad/glad.h>

namespace {

struct PortTitleActor : public fopAc_ac_c {
    u32 frame;
    u32 acceptedFrame;
    bool accepted;
};

static int PortTitle_Create(fopAc_ac_c* i_this) {
    auto* actor = static_cast<PortTitleActor*>(i_this);
    fopAcM_ct(actor, PortTitleActor);
    actor->frame = 0;
    actor->acceptedFrame = 0;
    actor->accepted = false;
    tp::log::info("PortTitleActor: create id=%d", fopAcM_GetID(actor));
    return 1;
}

static int PortTitle_Delete(PortTitleActor* /*i_this*/) {
    return 1;
}

static int PortTitle_Execute(PortTitleActor* i_this) {
    ++i_this->frame;

    JUTGamePad* pad = JUTGamePad::getGamePad(0);
    if (!i_this->accepted && pad != nullptr &&
        pad->testTrigger(PAD_BUTTON_START | PAD_BUTTON_A | PAD_BUTTON_B))
    {
        i_this->accepted = true;
        i_this->acceptedFrame = 0;
        tp::log::info("PortTitleActor: start accepted buttons=0x%X", pad->getTrigger());
    }

    if (i_this->accepted) {
        ++i_this->acceptedFrame;
    }

    return 1;
}

static int PortTitle_Draw(PortTitleActor* i_this) {
    int width = 0;
    int height = 0;
    tp::window::GetFramebufferSize(&width, &height);

    const float fade = i_this->frame < 90 ? (i_this->frame / 89.0f) : 1.0f;
    const float pulse = 0.5f + 0.5f * ((i_this->frame % 120) / 119.0f);
    const float acceptPulse =
        i_this->accepted ? (0.5f + 0.5f * ((i_this->acceptedFrame % 24) / 23.0f)) : 0.0f;
    const int titleBandY = (height * 58) / 100;
    const int titleBandH = height / 10;
    const int logoGlowY = (height * 52) / 100;
    const int logoGlowH = height / 4;
    const int logoGlowX = width / 7;
    const int logoGlowW = (width * 5) / 7;
    const int crestY = (height * 61) / 100;
    const int crestH = height / 22;
    const int crestX = width / 5;
    const int crestW = (width * 3) / 5;
    const int pressY = (height * 24) / 100;
    const int pressH = height / 16;
    const int pressX = width / 3;
    const int pressW = width / 3;
    const int vignette = width / 10;

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_SCISSOR_TEST);

    glScissor(logoGlowX, logoGlowY, logoGlowW, logoGlowH);
    glClearColor(0.30f + 0.20f * pulse, 0.24f + 0.10f * pulse, 0.38f + 0.18f * pulse, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glScissor(width / 8, titleBandY, (width * 3) / 4, titleBandH);
    glClearColor(0.74f + 0.14f * acceptPulse, 0.63f + 0.12f * acceptPulse,
                 0.34f + 0.26f * acceptPulse, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glScissor(crestX, crestY, crestW, crestH);
    glClearColor(0.92f, 0.88f, 0.70f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glScissor(pressX, pressY, pressW, pressH);
    glClearColor(0.95f, 0.84f + 0.08f * acceptPulse, 0.45f,
                 0.45f + 0.45f * (i_this->accepted ? 1.0f : pulse));
    glClear(GL_COLOR_BUFFER_BIT);

    glScissor(0, 0, vignette, height);
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glScissor(width - vignette, 0, vignette, height);
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    if (i_this->frame <= 3) {
        tp::log::info("PortTitleActor: draw frame=%u size=%dx%d", i_this->frame, width, height);
    }

    return 1;
}

static actor_method_class l_portTitleMethod = {
    (process_method_func)PortTitle_Create,
    (process_method_func)PortTitle_Delete,
    (process_method_func)PortTitle_Execute,
    (process_method_func)NULL,
    (process_method_func)PortTitle_Draw,
};

}  // namespace

actor_process_profile_definition g_profile_TITLE = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_TITLE_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(PortTitleActor),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_TITLE_e,
    /* Actor SubMtd */ &l_portTitleMethod,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x4000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

#endif
