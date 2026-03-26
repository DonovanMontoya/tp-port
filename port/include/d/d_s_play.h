#pragma once
/**
 * port/include/d/d_s_play.h
 * PC port stub — dScnPlay scene class.
 * Includes m_Do_hostIO.h because m_Do_main.cpp uses mDoHIO_entry_c (defined
 * there) and relies on d_s_play.h pulling it in transitively.
 */
#ifndef D_S_D_S_PLAY_H
#define D_S_D_S_PLAY_H

#include "m_Do/m_Do_hostIO.h"
#include "SSystem/SComponent/c_phase.h"
#include "f_pc/f_pc_manager.h"

class dScnPlay_c {
public:
    static void create() {}
    static bool isPause() { return false; }
};

// Alias used by f_op_msg.cpp
typedef dScnPlay_c dScnPly_c;

// Debug register macros — all zero on PC (no HIO hardware)
#ifndef TREG_F
#define TREG_F(i)  (0.0f)
#endif
#ifndef IREG_F
#define IREG_F(i)  (0.0f)
#endif
#ifndef AREG_F
#define AREG_F(i)  (0.0f)
#endif

#endif /* D_S_D_S_PLAY_H */
