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
#include "JSystem/JKernel/JKRHeap.h"

class JKRArchive;
class JKRSolidHeap;

class Z2AudioMgr : public JASGlobalInstance<Z2AudioMgr> {
public:
    Z2AudioMgr() : JASGlobalInstance<Z2AudioMgr>(true) {}

    static Z2AudioMgr* getInterface() {
        return JASGlobalInstance<Z2AudioMgr>::getInstance();
    }

    void init(JKRSolidHeap* /*heap*/, u32 /*memSize*/, void* /*baaData*/, JKRArchive* /*seqArc*/) {}
    void setEventBit(u32 /*bits*/) {}
    void gframeProcess() {}
    void setOutputMode(u32 /*mode*/) {}
    void setSceneName(char* /*spot*/, s32 /*room*/, s32 /*layer*/) {}
    void load1stDynamicWave() {}
    void loadStaticWaves() {}
    void setFadeOutStart(u8 /*param*/) {}
    void setFadeInStart(u8 /*param*/) {}
    void resetProcess(u32 /*param*/, bool /*hard*/) {}
    void resetRecover() {}
    bool hasReset() const { return true; }
    bool checkFirstWaves() { return true; }
};

inline Z2AudioMgr* Z2GetAudioMgr() {
    return JASGlobalInstance<Z2AudioMgr>::getInstance();
}

#endif /* Z2AUDIOMGR_H */
