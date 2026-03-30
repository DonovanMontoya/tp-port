#pragma once
/**
 * port/include/JSystem/JFramework/JFWDisplay.h
 * PC port stub — JFWDisplay (GC display manager).
 * On PC there's no EFB/XFB; stub provides interface compatibility.
 */
#ifndef JFWDISPLAY_H
#define JFWDISPLAY_H

#include "port/types.h"
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/JUtility/JUTXfb.h"

// Forward declarations / minimal stubs to avoid pulling in GC headers
class JKRHeap;

#include "JSystem/JUtility/TColor.h"

// Minimal JUTFader stub — enough for m_Do_graphic.cpp bring-up.
class JUTFader {
public:
    enum {
        UNKSTATUS_0 = 0,
        UNKSTATUS_1 = 1,
        UNKSTATUS_2 = 2,
    };

    JUTFader() = default;
    JUTFader(int /*x*/, int /*y*/, int /*w*/, int /*h*/, JUtility::TColor color) : mColor(color) {}

    void setColor(JUtility::TColor c) { mColor = c; }
    void setStatus(int status, int frames) {
        mStatus = status;
        mFramesRemaining = frames > 0 ? frames : 0;
    }
    int getStatus() {
        tick();
        return mStatus;
    }
    bool startFadeIn(int frames) {
        if (frames <= 0) {
            mStatus = UNKSTATUS_0;
            mFramesRemaining = 0;
            return false;
        }
        if (mStatus != UNKSTATUS_0 || mFramesRemaining > 0) {
            tick();
            return mStatus != UNKSTATUS_0;
        }
        mStatus = UNKSTATUS_0;
        mFramesRemaining = frames;
        tick();
        return mStatus != UNKSTATUS_0;
    }
    bool startFadeOut(int frames) {
        mStatus = UNKSTATUS_1;
        mFramesRemaining = frames > 0 ? frames : 0;
        return true;
    }
    bool isFinished() {
        tick();
        return mFramesRemaining == 0;
    }

private:
    void tick() {
        if (mFramesRemaining > 0) {
            --mFramesRemaining;
            if (mFramesRemaining == 0) {
                mStatus = (mStatus == UNKSTATUS_1) ? UNKSTATUS_2 : UNKSTATUS_0;
            }
        }
    }

    JUtility::TColor mColor = {};
    int mStatus = UNKSTATUS_0;
    int mFramesRemaining = 0;
};

struct _GXRenderModeObj;
typedef _GXRenderModeObj GXRenderModeObj;
struct _GXColor;
typedef _GXColor GXColor;

typedef void (*JFWDisplayUnkFunc)(void);

class JFWDisplay {
public:
    enum EDrawDone {
        UNK_METHOD_0 = 0,
        UNK_METHOD_1 = 1,
    };

    static JFWDisplay* createManager(const GXRenderModeObj* /*mode*/, JKRHeap* /*heap*/,
                                     int /*xfbNum*/, bool /*v*/) {
        if (!sManager) sManager = new JFWDisplay();
        return sManager;
    }
    static JFWDisplay* createManager(JKRHeap* heap, int xfbNum, bool v) {
        return createManager(nullptr, heap, xfbNum, v);
    }
    static JFWDisplay* getManager() { return sManager; }

    virtual void beginRender()  {}
    virtual void endRender()    {}
    virtual void endFrame()     {}
    virtual ~JFWDisplay()       {}

    void prepareCopyDisp()      {}
    void preGX()                {}
    void endGX()                {}
    void waitBlanking(int)      {}
    void clearEfb_init()        {}
    void clearEfb()             {}
    void clearEfb(int,int,int,int,GXColor) {}
    void clearEfb(GXColor)      {}

    bool startFadeOut(int)      { return true; }
    int  startFadeIn(int)       { return 1; }

    void setTickRate(u32 /*r*/)     {}
    void setFrameRate(u16 /*r*/)    {}
    void setDrawDoneMethod(EDrawDone /*d*/) {}
    void setFader(JUTFader* f)      { mFader = f; }
    void resetFader()               { mFader = nullptr; }
    JUTFader* getFader() const      { return mFader; }
    void setClearColor(JUtility::TColor /*c*/) {}
    int  getEfbWidth()  const       { return 640; }
    int  getEfbHeight() const       { return 480; }
    JUTXfb* getXfbManager() const   { return nullptr; }

    static JFWDisplay* sManager;

private:
    JUTFader* mFader = nullptr;
};

static inline void JUTChangeFrameBuffer(void* /*buf*/, u16 /*w*/, u16 /*h*/) {}

#endif /* JFWDISPLAY_H */
