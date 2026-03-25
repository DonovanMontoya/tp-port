#pragma once
/**
 * port/include/d/d_s_menu.h
 * PC port stub — dScnMenu scene class (save-select / debug options).
 */
#ifndef D_S_D_S_MENU_H
#define D_S_D_S_MENU_H

#include "port/types.h"
#include <cstring>

class dScnMenu_c {
public:
    static int  cursolPoint;
    static int  cursolLayer;
    static int  cursolRoomNo;
    static char cursolStageName[32];
    static u8   m_error_flags;
};

#endif /* D_S_D_S_MENU_H */
