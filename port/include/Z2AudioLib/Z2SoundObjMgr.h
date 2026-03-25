#pragma once
#ifndef Z2SOUNDOBJMGR_H
#define Z2SOUNDOBJMGR_H
#include "JSystem/JAudio2/JASGadget.h"
class Z2SoundObjMgr : public JASGlobalInstance<Z2SoundObjMgr> {
public:
    Z2SoundObjMgr() : JASGlobalInstance<Z2SoundObjMgr>(true) {}
};
#endif
