#pragma once
#ifndef Z2FXLINEMGR_H
#define Z2FXLINEMGR_H
#include "JSystem/JAudio2/JASGadget.h"
class Z2FxLineMgr : public JASGlobalInstance<Z2FxLineMgr> {
public:
    Z2FxLineMgr() : JASGlobalInstance<Z2FxLineMgr>(true) {}
};
#endif
