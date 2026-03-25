#pragma once
#ifndef Z2AUDIENCE_H
#define Z2AUDIENCE_H
#include "JSystem/JAudio2/JASGadget.h"
class Z2Audience : public JASGlobalInstance<Z2Audience> {
public:
    Z2Audience() : JASGlobalInstance<Z2Audience>(true) {}
};
#endif
