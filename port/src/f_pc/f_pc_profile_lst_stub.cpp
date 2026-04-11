/**
 * port/src/f_pc/f_pc_profile_lst_stub.cpp
 * PC port replacement for f_pc/f_pc_profile_lst.cpp.
 *
 * The original file references hundreds of game-specific profile objects
 * (scenes, actors, kankyo, etc.) that are not yet compiled.  This stub
 * provides a null-terminated profile table so the linker is satisfied.
 *
 * On PC there is no REL loader so ModuleProlog is never called; instead
 * we use a static constructor to set g_fpcPf_ProfileList_p at startup.
 * The framework indexes this table by raw process-name enum value, so the
 * array must preserve those numeric slots even when most entries are null.
 */
#include "f_pc/f_pc_profile.h"
#include "f_pc/f_pc_name.h"
#include "f_pc/f_pc_profile_lst.h"
#include "port/boot_scene.h"

namespace {
constexpr int kProfileTableSize = fpcNm_MAX_NUM;
process_profile_definition* s_ProfileList[kProfileTableSize] = {};
}

namespace {
struct ProfileListInit {
    ProfileListInit() {
        s_ProfileList[fpcNm_OVERLAP0_e] = &g_profile_OVERLAP0;
        s_ProfileList[fpcNm_OVERLAP1_e] = &g_profile_OVERLAP1;
        s_ProfileList[fpcNm_OVERLAP2_e] = &g_profile_OVERLAP2;
        s_ProfileList[fpcNm_OVERLAP3_e] = &g_profile_OVERLAP3;
        s_ProfileList[fpcNm_OVERLAP6_e] = &g_profile_OVERLAP6;
        s_ProfileList[fpcNm_OVERLAP7_e] = &g_profile_OVERLAP7;
        s_ProfileList[fpcNm_OVERLAP8_e] = &g_profile_OVERLAP8;
        s_ProfileList[fpcNm_OVERLAP9_e] = &g_profile_OVERLAP9;
        s_ProfileList[fpcNm_OVERLAP10_e] = &g_profile_OVERLAP10;
        s_ProfileList[fpcNm_OVERLAP11_e] = &g_profile_OVERLAP11;
        s_ProfileList[fpcNm_LOGO_SCENE_e] = &g_profile_LOGO_SCENE;
        s_ProfileList[fpcNm_MENU_SCENE_e] =
            reinterpret_cast<process_profile_definition*>(&g_profile_PORT_MENU_SCENE);
        s_ProfileList[fpcNm_PLAY_SCENE_e] = &g_profile_PLAY_SCENE;
        s_ProfileList[fpcNm_ROOM_SCENE_e] = &g_profile_ROOM_SCENE;
        s_ProfileList[fpcNm_OPENING_SCENE_e] = &g_profile_OPENING_SCENE;
        s_ProfileList[fpcNm_TITLE_e] = &g_profile_TITLE;
        s_ProfileList[fpcNm_WARNING_SCENE_e] = &g_profile_WARNING_SCENE;
        s_ProfileList[fpcNm_WARNING2_SCENE_e] = &g_profile_WARNING2_SCENE;
        s_ProfileList[fpcNm_ALINK_e] = &g_profile_ALINK;
        s_ProfileList[fpcNm_CAMERA_e] = &g_profile_CAMERA;
        s_ProfileList[fpcNm_CAMERA2_e] = &g_profile_CAMERA2;
        s_ProfileList[fpcNm_METER2_e] = &g_profile_METER2;
        g_fpcPf_ProfileList_p = s_ProfileList;
    }
} s_profileListInit;
}

extern "C" void ModuleProlog() {
    g_fpcPf_ProfileList_p = s_ProfileList;
}

extern "C" void ModuleEpilog() {
    // no-op on PC
}

extern "C" process_profile_definition** tpPort_GetProfileList() {
    return s_ProfileList;
}
