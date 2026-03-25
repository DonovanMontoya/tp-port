#pragma once
#ifndef Z2SPEECHMGR2_H
#define Z2SPEECHMGR2_H
#include "JSystem/JAudio2/JASGadget.h"
class Z2SpeechMgr2 : public JASGlobalInstance<Z2SpeechMgr2> {
public:
    Z2SpeechMgr2() : JASGlobalInstance<Z2SpeechMgr2>(true) {}
};
#endif
