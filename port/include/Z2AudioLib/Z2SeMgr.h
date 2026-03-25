#pragma once
#ifndef Z2SEMGR_H
#define Z2SEMGR_H
#include "JSystem/JAudio2/JASGadget.h"
class Z2SeMgr : public JASGlobalInstance<Z2SeMgr> {
public:
    Z2SeMgr() : JASGlobalInstance<Z2SeMgr>(true) {}
};
#endif
