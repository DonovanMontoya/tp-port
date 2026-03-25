#pragma once
#ifndef Z2STATUSMGR_H
#define Z2STATUSMGR_H
#include "JSystem/JAudio2/JASGadget.h"
class Z2StatusMgr : public JASGlobalInstance<Z2StatusMgr> {
public:
    Z2StatusMgr() : JASGlobalInstance<Z2StatusMgr>(true) {}
};
#endif
