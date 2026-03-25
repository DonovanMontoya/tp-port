#pragma once
/**
 * port/include/JSystem/JAWExtSystem/JAWExtSystem.h
 * PC port stub — JAWExtSystem (debug window/overlay manager).
 * All functions are no-ops; no rendering on PC.
 */
#include "port/types.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRThread.h"

// Forward declarations
class JAWWindow;
struct JUTGamePad;

// Minimal JAWSystemInterface stub
class JAWSystemInterface {
public:
    virtual ~JAWSystemInterface() {}
    virtual BOOL registWindow(u32, JAWWindow*, int, int) { return FALSE; }
    virtual BOOL destroyWindow(u32, JAWWindow*)          { return FALSE; }
    virtual void setForegroundWindow(JAWWindow*)         {}
    JKRHeap* getCurrentHeap() const { return JKRHeap::sCurrentHeap; }
};

namespace JAWExtSystem {

    inline BOOL registWindow(u32, JAWWindow*, int, int) { return FALSE; }
    inline BOOL destroyWindow(u32, JAWWindow*)          { return FALSE; }
    inline void nextPage()    {}
    inline void prevPage()    {}
    inline void nextWindow()  {}
    inline void prevWindow()  {}
    inline void draw()        {}
    inline void padProc(const JUTGamePad&) {}

    class TSystemInterface : public JAWSystemInterface {
    public:
        TSystemInterface() {}
        virtual BOOL registWindow(u32 p1, JAWWindow* p2, int p3, int p4) override {
            return JAWExtSystem::registWindow(p1, p2, p3, p4);
        }
        virtual BOOL destroyWindow(u32 p1, JAWWindow* p2) override {
            return JAWExtSystem::destroyWindow(p1, p2);
        }
        virtual void setForegroundWindow(JAWWindow*) override {}
    };

    class TCurrentHeap {
    public:
        TCurrentHeap(const TSystemInterface& iface) {
            heap = JKRGetCurrentHeap();
            JKRSetCurrentHeap(iface.getCurrentHeap());
        }
        ~TCurrentHeap() { JKRSetCurrentHeap(heap); }
        JKRHeap* heap;
    };

    // Stubs for global data referenced in the original
    extern s32 sCurrentPage;
    extern u8  lbl_80748E44;
    extern TSystemInterface sInterface;

} // namespace JAWExtSystem
