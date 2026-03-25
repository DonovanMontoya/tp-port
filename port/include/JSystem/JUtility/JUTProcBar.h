#pragma once
/**
 * port/include/JSystem/JUtility/JUTProcBar.h
 * PC port stub — process/GPU timing bar used for profiling.
 * All methods are no-ops; there is no VI/GX on PC.
 */
#include "JSystem/JUtility/TColor.h"
#include "port/types.h"

class JKRHeap;

class JUTProcBar {
public:
    class CTime {
    public:
        CTime() { clear(); }
        void clear()                        { mCost = 0; field_0x8 = 0; field_0xc = 0; }
        void start(u8, u8, u8)              {}
        void end()                          {}
        void accumePeek()                   {}
        int  calcBarSize(int, int)    const { return 0; }

        u32 mTick   = 0;
        u32 mCost   = 0;
        u32 field_0x8 = 0;
        u32 field_0xc = 0;
        u8  mR = 0, mG = 0, mB = 0;
    };

    class CParamSet {
    public:
        int mBarWidth    = 0;
        int mPosX        = 0;
        int mPosY        = 0;
        int mWidth       = 0;
        int mUserPosition = 0;

        CParamSet() {}
        void setBarWidth(int w)          { mBarWidth = w; }
        void setPosition(int x, int y)   { mPosX = x; mPosY = y; }
        void setWidth(int w)             { mWidth = w; }
        void setUserPosition(int p)      { mUserPosition = p; }
    };

    JUTProcBar()  {}
    ~JUTProcBar() {}

    static JUTProcBar* create()  { if (!sManager) sManager = new JUTProcBar(); return sManager; }
    static void        destroy() { delete sManager; sManager = nullptr; }
    static void        clear()   {}

    void bar_subroutine(int, int, int, int, int, int, int,
                        JUtility::TColor, JUtility::TColor) {}
    void adjustMeterLength(u32, f32*, f32, f32, int*)       {}
    void draw()             {}
    void drawProcessBar()   {}
    void drawHeapBar()      {}

    void cpuStart()          {}
    void cpuEnd()            {}
    void gpWaitStart()       {}
    void gpWaitEnd()         {}
    void gpStart()           {}
    void gpEnd()             {}
    void wholeLoopStart()    {}
    void wholeLoopEnd()      {}
    void idleStart()         {}
    void idleEnd()           {}
    void setCostFrame(int)   {}
    void setVisible(bool v)          { mVisible = v; }
    void setVisibleHeapBar(bool v)   { mHeapBarVisible = v; }
    void setWatchHeap(JKRHeap* p)    { mWatchHeap = p; }

    static JUTProcBar* getManager() { return sManager; }
    static JUTProcBar* sManager;

private:
    CTime    mIdle, mGp, mCpu, mGpWait, mWholeLoop;
    CTime    mUsers[8];
    int      mCostFrame   = 0;
    int      field_0x108  = 0;
    bool     mVisible     = false;
    int      field_0x110  = 0;
    CParamSet mParams;
    int      field_0x128  = 0;
    JKRHeap* mWatchHeap   = nullptr;
    bool     mHeapBarVisible = false;
};
