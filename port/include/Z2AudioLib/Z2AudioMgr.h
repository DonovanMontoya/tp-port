#pragma once
/**
 * port/include/Z2AudioLib/Z2AudioMgr.h
 * PC port stub — audio manager.
 * Shadows the original which pulls in many Z2/JAS headers.
 * f_pc_base.cpp calls Z2GetAudioMgr() only inside #if DEBUG.
 */
#ifndef Z2AUDIOMGR_H
#define Z2AUDIOMGR_H

#include "JSystem/JAudio2/JASGadget.h"

class Z2AudioMgr : public JASGlobalInstance<Z2AudioMgr> {
public:
    Z2AudioMgr() : JASGlobalInstance<Z2AudioMgr>(true) {}
};

inline Z2AudioMgr* Z2GetAudioMgr() {
    return JASGlobalInstance<Z2AudioMgr>::getInstance();
}

#endif /* Z2AUDIOMGR_H */
