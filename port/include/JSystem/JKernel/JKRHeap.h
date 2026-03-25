/**
 * port/include/JSystem/JKernel/JKRHeap.h
 * PC port stub — wraps malloc/free to satisfy the GC heap API surface.
 *
 * JKRHeap is the base heap class used throughout the game for all dynamic
 * allocation.  On PC we implement alloc/free with the system allocator and
 * keep the interface identical to avoid touching game source.
 */
#pragma once
#include "JSystem/JKernel/JKRDisposer.h"
#include "port/types.h"
#include <cstdlib>
#include <new>

// Suppress original GC header (dolphin/os.h brought in by the GC version)
#ifndef _DOLPHIN_OS_H_
#define _DOLPHIN_OS_H_FORWARDED
#endif

class JKRHeap : public JKRDisposer {
public:
    // ----- types -----
    typedef void (*JKRAllocCallback)(u32, int, JKRHeap*, void*);
    typedef void (*JKRFreeCallback)(void*, JKRHeap*);
    typedef void (*JKRErrorHandler)(void*, u32, int);

    class TState {
    public:
        u32      mUsedSize  = 0;
        u32      mCheckCode = 0;
        u32      mBuf       = 0;
        u32      mField0C   = 0;
        JKRHeap* mHeap      = nullptr;
        u32      mId        = 0;

        u32      getUsedSize()  const { return mUsedSize; }
        u32      getCheckCode() const { return mCheckCode; }
        JKRHeap* getHeap()      const { return mHeap; }
        u32      getId()        const { return mId; }
    };

    // ----- construction -----
    JKRHeap(u32 /*size*/, JKRHeap* parent, bool /*errFlag*/) : JKRDisposer(), mParent(parent) {}
    JKRHeap(void* /*data*/, u32 /*size*/, JKRHeap* parent, bool /*errFlag*/) : JKRDisposer(), mParent(parent) {}
    virtual ~JKRHeap() {}

    // ----- core allocation -----
    virtual void* alloc(u32 size, int /*alignment*/ = 4) { return ::malloc(size); }
    virtual void  free(void* ptr)                         { ::free(ptr); }
    virtual void  freeAll()                               {}
    virtual void  freeTail()                              {}

    // ----- state / info -----
    u32  getFreeSize()   const { return 0x10000000u; }
    u32  getTotalFreeSize() const { return 0x10000000u; }
    u32  getUsedSize()   const { return 0; }
    bool isValid()       const { return true; }
    JKRHeap* getParent() const { return mParent; }

    // ----- global system heap -----
    static JKRHeap* sSystemHeap;
    static JKRHeap* sCurrentHeap;
    static JKRHeap* getRootHeap()    { return sSystemHeap; }
    static JKRHeap* getCurrentHeap() { return sCurrentHeap; }

    JKRHeap* becomeSystemHeap() {
        JKRHeap* prev = sSystemHeap;
        sSystemHeap   = this;
        return prev;
    }
    JKRHeap* becomeCurrentHeap() {
        JKRHeap* prev = sCurrentHeap;
        sCurrentHeap  = this;
        return prev;
    }

    // ----- error handler -----
    static void setErrorHandler(JKRErrorHandler /*h*/) {}

    // ----- save/check state (no-op stubs) -----
    void          makeState(TState* s)         const { if (s) { s->mHeap = const_cast<JKRHeap*>(this); } }
    bool          checkState(TState* /*s*/)    const { return true; }
    static void   setState_(TState*)           {}

    // ----- debug fill values (no-op on PC) -----
    void      setDebugFill(bool /*b*/)  {}
    bool      getDebugFill()   const { return false; }

    // ----- aligned alloc helper -----
    void* allocFromHead(u32 size, int align = 4) { return alloc(size, align); }
    void* allocFromTail(u32 size, int align = 4) { return alloc(size, align); }

    // ----- operator new/delete for heap objects -----
    static void* operator_new(u32 size, JKRHeap* heap, int align = 4) {
        return heap ? heap->alloc(size, align) : ::malloc(size);
    }

private:
    JKRHeap* mParent;
};

// Global stub values for debug fill (referenced by some game files)
inline u8 JKRValue_DEBUGFILL_NOTUSE = 0xAA;
inline u8 JKRValue_DEBUGFILL_NEW    = 0xBB;
inline u8 JKRValue_DEBUGFILL_DELETE = 0xCC;
inline s32 fillcheck_dispcount      = 0;
inline bool data_8074A8D0_debug     = false;

// -----------------------------------------------------------------------
// JKRSolidHeap — simple sub-heap that carves from a flat buffer
// -----------------------------------------------------------------------
class JKRSolidHeap : public JKRHeap {
public:
    static JKRSolidHeap* create(u32 size, JKRHeap* parent, bool errorFlag) {
        void* buf = parent ? parent->alloc(sizeof(JKRSolidHeap) + size, 4) : ::malloc(sizeof(JKRSolidHeap) + size);
        return new(buf) JKRSolidHeap(buf, size, parent, errorFlag);
    }
    JKRSolidHeap(void* buf, u32 size, JKRHeap* parent, bool errFlag) : JKRHeap(buf, size, parent, errFlag) {}
};

// -----------------------------------------------------------------------
// JKRExpHeap — expandable heap (same as JKRHeap on PC)
// -----------------------------------------------------------------------
class JKRExpHeap : public JKRHeap {
public:
    static JKRExpHeap* create(u32 size, JKRHeap* parent, bool errorFlag) {
        return new JKRExpHeap(size, parent, errorFlag);
    }
    static JKRExpHeap* createRoot(int /*id*/, bool /*errorFlag*/) {
        return new JKRExpHeap(0, nullptr, false);
    }
    JKRExpHeap(u32 size, JKRHeap* parent, bool errFlag) : JKRHeap(size, parent, errFlag) {}
};
