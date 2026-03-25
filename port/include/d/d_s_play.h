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

class dScnPlay_c {
public:
    static void create() {}
};

#endif /* D_S_D_S_PLAY_H */
