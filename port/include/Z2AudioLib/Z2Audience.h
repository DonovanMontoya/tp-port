#pragma once
#ifndef Z2AUDIENCE_H
#define Z2AUDIENCE_H
#include "JSystem/JAudio2/JASGadget.h"
#include "port/types.h"
class Z2Audience : public JASGlobalInstance<Z2Audience> {
public:
    Z2Audience() : JASGlobalInstance<Z2Audience>(true) {}
    void setAudioCamera(f32 (*)[4], Vec&, Vec&, f32, f32, bool, int, bool) {}
};

inline Z2Audience* Z2GetAudience() {
    return JASGlobalInstance<Z2Audience>::getInstance();
}
#endif
