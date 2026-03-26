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
 * fpcPf_Get() is only called when a process is actually created (which
 * requires scene files to be enabled), so the empty list is safe for now.
 */
#include "f_pc/f_pc_profile.h"
#include "f_pc/f_pc_profile_lst.h"

// Profiles for scene/actor files that are now compiled.
// Add entries here as more files are enabled in game_sources.cmake.
static process_profile_definition* s_ProfileList[] = {
    &g_profile_LOGO_SCENE,
    &g_profile_WARNING_SCENE,
    &g_profile_WARNING2_SCENE,
    nullptr,
};

// Set the profile list pointer before main() via a static initializer
namespace {
struct ProfileListInit {
    ProfileListInit() {
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
