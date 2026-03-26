#pragma once
/**
 * port/include/c/c_dylink.h
 * PC port stub — REL dynamic linking. On PC there are no REL files.
 */
#ifndef C_C_DYLINK_H
#define C_C_DYLINK_H

#include "port/types.h"

struct request_of_phase_process_class;

struct cDylPhs {
    static int Link(request_of_phase_process_class*, s16) { return 1; }
    static int Unlink(request_of_phase_process_class*, s16) { return 1; }
};

inline int  cDyl_InitAsyncIsDone() { return 1; }
inline void cDyl_InitAsync()       {}
inline BOOL cDyl_IsLinked(s16)     { return TRUE; }
inline BOOL cDyl_Unlink(s16)       { return TRUE; }
inline int  cDyl_LinkASync(s16)    { return 0; }
inline BOOL cCc_Check()            { return TRUE; }

#endif /* C_C_DYLINK_H */
