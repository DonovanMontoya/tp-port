#pragma once
#ifndef Z2ENVSEMGR_H
#define Z2ENVSEMGR_H
#include "JSystem/JAudio2/JASGadget.h"
class Z2EnvSeMgr : public JASGlobalInstance<Z2EnvSeMgr> {
public:
    Z2EnvSeMgr() : JASGlobalInstance<Z2EnvSeMgr>(true) {}
};
#endif
