#pragma once
#ifndef Z2SOUNDMGR_H
#define Z2SOUNDMGR_H
#include "JSystem/JAudio2/JASGadget.h"
class Z2SoundMgr : public JASGlobalInstance<Z2SoundMgr> {
public:
    Z2SoundMgr() : JASGlobalInstance<Z2SoundMgr>(true) {}
};
#endif
