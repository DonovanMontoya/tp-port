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

// Forward declarations / minimal stubs to avoid pulling in GC headers
class JKRHeap;
class JUTVideo;

#include "JSystem/JUtility/TColor.h"

// Minimal JUTFader stub — m_Do_graphic.h calls setColor/startFadeIn/startFadeOut
class JUTFader {
public:
    void setColor(JUtility::TColor /*c*/) {}
    bool startFadeIn(int /*frames*/)       { return true; }
    bool startFadeOut(int /*frames*/)      { return true; }
    bool isFinished()                      { return true; }
};

// Minimal JUTXfb stub — m_Do_graphic.h calls getDisplayingXfb
class JUTXfb {
public:
    void* getDisplayingXfb() const { return nullptr; }
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
