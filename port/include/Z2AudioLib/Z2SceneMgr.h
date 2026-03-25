#pragma once
#ifndef Z2SCENEMGR_H
#define Z2SCENEMGR_H
#include "JSystem/JAudio2/JASGadget.h"
class Z2SceneMgr : public JASGlobalInstance<Z2SceneMgr> {
public:
    Z2SceneMgr() : JASGlobalInstance<Z2SceneMgr>(true) {}
};
#endif
