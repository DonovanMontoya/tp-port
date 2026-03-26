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

// Empty profile table — extended as scene/actor files are enabled
static process_profile_definition* s_EmptyProfileList[] = {
    nullptr,
};

// Set the profile list pointer before main() via a static initializer
namespace {
struct ProfileListInit {
    ProfileListInit() {
        g_fpcPf_ProfileList_p = s_EmptyProfileList;
    }
} s_profileListInit;
}

extern "C" void ModuleProlog() {
    g_fpcPf_ProfileList_p = s_EmptyProfileList;
}

extern "C" void ModuleEpilog() {
    // no-op on PC
}
