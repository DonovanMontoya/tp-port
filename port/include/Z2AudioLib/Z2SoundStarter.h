#pragma once
#ifndef Z2SOUNDSTARTER_H
#define Z2SOUNDSTARTER_H
#include "JSystem/JAudio2/JASGadget.h"
class Z2SoundStarter : public JASGlobalInstance<Z2SoundStarter> {
public:
    Z2SoundStarter() : JASGlobalInstance<Z2SoundStarter>(true) {}
};
#endif
