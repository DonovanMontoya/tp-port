#pragma once
/**
 * port/include/JSystem/JAudio2/JASAudioThread.h
 * PC port stub — GameCube DSP audio thread.
 * Audio is handled by SDL/OpenAL on PC; this is a no-op wrapper.
 */
#include "JSystem/JKernel/JKRThread.h"
#include "dolphin/os/OSThread.h"

enum JASAudioMessage {
    AUDIOMSG_DMA  = 0,
    AUDIOMSG_DSP  = 1,
    AUDIOMSG_STOP = 2,
};

// Minimal JASGlobalInstance stub (the real one is in JAudio2/JASGadget.h)
template <typename T>
struct JASGlobalInstance {
    static T* getInstance()      { return s_instance; }
    static void setInstance(T* p){ s_instance = p; }
protected:
    JASGlobalInstance(bool /*primary*/) {}
    static T* s_instance;
};
template <typename T> T* JASGlobalInstance<T>::s_instance = nullptr;

struct JASAudioThread : public JKRThread, public JASGlobalInstance<JASAudioThread> {
    JASAudioThread(int /*prio*/, int /*msgCount*/, u32 /*stackSize*/)
        : JKRThread(static_cast<OSThread*>(nullptr), 0)
        , JASGlobalInstance<JASAudioThread>(true)
        , sbPauseFlag(false)
    {
        JASGlobalInstance<JASAudioThread>::setInstance(this);
    }
    virtual ~JASAudioThread() {}

    static void create(s32 /*prio*/) {}
    void        stop()               {}
    virtual void* run() override     { return nullptr; }
    static void DMACallback()        {}
    static void DSPCallback(void*)   {}

    static JASAudioThread* getThreadPointer() { return getInstance(); }
    static int             getDSPSyncCount()  { return snIntCount; }
    static void            setDSPSyncCount(u32 v) { snIntCount = (int)v; }

    OSThreadQueue    sThreadQueue;
    bool             sbPauseFlag;
    static volatile int snIntCount;
};
