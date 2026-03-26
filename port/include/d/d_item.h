#pragma once
/**
 * port/include/d/d_item.h
 * PC port stub — item system.
 * The original d/d_item.h only has dEnemyItem_c and item functions, but
 * f_op_actor_mng.cpp needs the dItemNo_* enum from d/d_item_data.h.
 */
#ifndef D_D_ITEM_H
#define D_D_ITEM_H

#include "port/types.h"
#include "d/d_item_data.h"

class dEnemyItem_c {
public:
    static void setItemData(u8* data) { mData = data; }
    static u8*  getItemData()         { return mData; }
    static u8*  mData;
};

// Item helpers used by fopAcM item creation functions
inline u8   check_itemno(int itemNo) { return (u8)(itemNo & 0xFF); }
inline BOOL isHeart(int itemNo)      { return itemNo == dItemNo_HEART_e ? TRUE : FALSE; }

#endif /* D_D_ITEM_H */
