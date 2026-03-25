#pragma once
#ifndef Z2SOUNDINFO_H
#define Z2SOUNDINFO_H
#include "JSystem/JAudio2/JASGadget.h"
class Z2SoundInfo : public JASGlobalInstance<Z2SoundInfo> {
public:
    Z2SoundInfo() : JASGlobalInstance<Z2SoundInfo>(true) {}
};
#endif
