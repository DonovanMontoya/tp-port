#pragma once
/**
 * port/include/JSystem/JUtility/JUTVideo.h
 * PC port stub — VI/GX video manager.
 * No VI retrace, no GX framebuffer on PC; all methods are no-ops returning safe defaults.
 */
#include "port/types.h"
#include "dolphin/os/OSTime.h"
#include "dolphin/os/OSMessage.h"

// Minimal forward declarations to avoid pulling in GX/VI headers
#ifndef _DOLPHIN_GX_GXSTRUCT_H_
struct _GXRenderModeObj;
typedef _GXRenderModeObj GXRenderModeObj;
#endif
typedef u8 (*Pattern)[2];

class JUTVideo {
public:
    typedef void (*Callback)(u32);

    JUTVideo(GXRenderModeObj const* mode) : mRenderObj(const_cast<GXRenderModeObj*>(mode)) {}
    virtual ~JUTVideo() {}

    static JUTVideo* createManager(GXRenderModeObj const* mode) {
        if (!sManager) sManager = new JUTVideo(mode);
        if (mode) sManager->setRenderMode(mode);
        return sManager;
    }
    static void destroyManager() { delete sManager; sManager = nullptr; }

    static void drawDoneStart()    {}
    static void dummyNoDrawWait()  {}

    void setRenderMode(GXRenderModeObj const* mode) { mRenderObj = const_cast<GXRenderModeObj*>(mode); }
    void waitRetraceIfNeed() {}

    static void preRetraceProc(u32)    {}
    static void postRetraceProc(u32)   {}
    static void drawDoneCallback()     {}

    u16 getFbWidth()     const { return 640; }
    u16 getEfbHeight()   const { return 480; }
    void getBounds(u16& w, u16& h) const { w = 640; h = 480; }
    u16 getXfbHeight()   const { return 480; }
    u8  isAntiAliasing() const { return 0; }
    Pattern getSamplePattern() const { return nullptr; }
    u8* getVFilter()     const { return nullptr; }
    OSMessageQueue* getMessageQueue() { return nullptr; }

    GXRenderModeObj* getRenderMode() const { return mRenderObj; }

    static JUTVideo* getManager()       { return sManager; }
    static OSTick    getVideoInterval() { return sVideoInterval; }
    static OSTick    getVideoLastTick() { return sVideoLastTick; }

private:
    inline static JUTVideo* sManager = nullptr;
    inline static OSTick    sVideoLastTick = 0;
    inline static OSTick    sVideoInterval = 0;

    GXRenderModeObj* mRenderObj;
};

inline JUTVideo* JUTGetVideoManager()  { return JUTVideo::getManager(); }
inline void      JUTDestroyVideoManager() { JUTVideo::destroyManager(); }
