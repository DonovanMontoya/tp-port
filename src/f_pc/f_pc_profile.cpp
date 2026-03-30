/**
 * f_pc_profile.cpp
 * Framework - Process Profile
 */

#include "f_pc/f_pc_profile.h"

process_profile_definition** g_fpcPf_ProfileList_p;

#if !defined(__REVOLUTION_SDK__)
extern "C" process_profile_definition** tpPort_GetProfileList();
#endif

process_profile_definition* fpcPf_Get(s16 i_profname) {
#if !defined(__REVOLUTION_SDK__)
    if (g_fpcPf_ProfileList_p == NULL) {
        g_fpcPf_ProfileList_p = tpPort_GetProfileList();
    }
#endif
    int index = i_profname;
    return g_fpcPf_ProfileList_p[index];
}
