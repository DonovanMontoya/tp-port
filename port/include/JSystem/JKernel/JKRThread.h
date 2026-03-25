#pragma once
/**
 * port/include/JSystem/JKernel/JKRThread.h
 * PC port shadow of the GC JKRThread class.
 * Keeps the full GC API surface; backs threads with std::thread on PC.
 */
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JSupport/JSUList.h"
#include "dolphin/os/OSThread.h"
#include "dolphin/os/OSMessage.h"
#include <thread>

// Forward declarations
class JUTConsole;

// ---------------------------------------------------------------------------
// JKRThreadName_ — thread name lookup table entry
// ---------------------------------------------------------------------------
struct JKRThreadName_ {
    s32   id;
    char* name;
};

// ---------------------------------------------------------------------------
// JKRThread
// ---------------------------------------------------------------------------
class JKRThread : public JKRDisposer {
public:
    // ---- TLoad inner class (timing / cost tracking) ----
    class TLoad {
    public:
        TLoad() { clear(); mValid = false; mThreadId = 0; }

        bool isValid()   const { return mValid; }
        u32  getCost()   const { return mCost; }
        u32  getCount()  const { return mSwitchCount; }
        s32  getId()     const { return mThreadId; }

        void setValid(bool valid) { mValid = valid; }
        void setId(s32 id)        { mThreadId = id; }
        void setCurrentTime()     { /* no GC timer on PC */ }

        void resetCost()  { mCost = 0; }
        void resetCount() { mSwitchCount = 0; }
        void incCount()   { mSwitchCount++; }
        void addCurrentCost() {}

        void clear() { resetCount(); resetCost(); }

    private:
        bool mValid;
        u8   padding_[3];
        u32  mCost;
        u32  mSwitchCount;
        u64  mLastTick;   // unused on PC
        s32  mThreadId;
    };

    // ---- Constructors / destructor ----
    JKRThread(u32 stack_size, int msg_count, int prio);
    JKRThread(JKRHeap* heap, u32 stack_size, int msg_count, int prio);
    JKRThread(OSThread* thread, int msg_count);
    virtual ~JKRThread();

    // ---- Virtual run entry point (override in subclasses) ----
    /* vt[03] */ virtual void* run() { return nullptr; }

    // ---- Setup helpers ----
    void setCommon_mesgQueue(JKRHeap* heap, int msg_count);
    void setCommon_heapSpecified(JKRHeap* heap, u32 stack_size, int prio);

    // ---- Accessors ----
    OSThread* getThreadRecord() const { return mThreadRecord; }
    void*     getStack()        const { return mStackMemory; }
    TLoad*    getLoadInfo()           { return &mLoadInfo; }
    JKRHeap*  getCurrentHeap()  const { return mCurrentHeap; }
    s32       getCurrentHeapError() const { return mCurrentHeapError; }

    void setCurrentHeap(JKRHeap* heap) {
        mCurrentHeap = heap ? heap : JKRHeap::getCurrentHeap();
    }

    // ---- Thread control ----
    void resume() { OSResumeThread(mThreadRecord); }

    BOOL sendMessage(OSMessage message) {
        return OSSendMessage(&mMessageQueue, message, OS_MESSAGE_NOBLOCK);
    }
    void sendMessageBlock(OSMessage message) {
        OSSendMessage(&mMessageQueue, message, OS_MESSAGE_BLOCK);
    }
    OSMessage waitMessage() {
        OSMessage msg;
        OSReceiveMessage(&mMessageQueue, &msg, OS_MESSAGE_NOBLOCK);
        return msg;
    }
    OSMessage waitMessage(BOOL* received) {
        OSMessage msg;
        BOOL rv = OSReceiveMessage(&mMessageQueue, &msg, OS_MESSAGE_NOBLOCK);
        if (received) *received = rv;
        return msg;
    }
    OSMessage waitMessageBlock() {
        OSMessage msg;
        OSReceiveMessage(&mMessageQueue, &msg, OS_MESSAGE_BLOCK);
        return msg;
    }
    void jamMessageBlock(OSMessage message) {
        OSJamMessage(&mMessageQueue, message, OS_MESSAGE_BLOCK);
    }

    // ---- Static helpers ----
    static void*      start(void* param);
    static JKRThread* searchThread(OSThread* thread);
    static JSUList<JKRThread>& getList() {
        return reinterpret_cast<JSUList<JKRThread>&>(sThreadList);
    }

    static JSUList<JKRThread> sThreadList;

private:
    JSULink<JKRThread> mThreadListLink;
    JKRHeap*           mHeap;
    OSThread*          mThreadRecord;
    OSMessageQueue     mMessageQueue;
    OSMessage*         mMesgBuffer;
    s32                mMessageCount;
    void*              mStackMemory;
    u32                mStackSize;
    TLoad              mLoadInfo;
    JKRHeap*           mCurrentHeap;
    s32                mCurrentHeapError;

    // PC-only backing
    std::thread* _thread;
    bool         _running;
};

// ---------------------------------------------------------------------------
// JKRIdleThread
// ---------------------------------------------------------------------------
class JKRIdleThread : public JKRThread {
public:
    virtual ~JKRIdleThread() { sThread = nullptr; }
    virtual void* run() { while (true) {} return nullptr; }

    static void* sThread;
};

// ---------------------------------------------------------------------------
// JKRThreadSwitch callback typedefs
// ---------------------------------------------------------------------------
typedef void (*JKRThreadSwitch_PreCallback)(OSThread* current, OSThread* next);
typedef void (*JKRThreadSwitch_PostCallback)(OSThread* current, OSThread* next);

// ---------------------------------------------------------------------------
// JKRThreadSwitch
// ---------------------------------------------------------------------------
class JKRThreadSwitch {
public:
    JKRThreadSwitch(JKRHeap* heap);
    virtual void draw(JKRThreadName_* names, JUTConsole* console);
    virtual void draw(JKRThreadName_* names) { draw(names, nullptr); }
    virtual ~JKRThreadSwitch() {}

    static JKRThreadSwitch* createManager(JKRHeap* heap);
    JKRThread* enter(JKRThread* t, int param);
    static void callback(OSThread* cur, OSThread* nxt);

    static JKRThreadSwitch* getManager()  { return sManager; }
    static u32              getTotalCount() { return sTotalCount; }

    static JKRThreadSwitch*            sManager;
    static u32                         sTotalCount;
    static u64                         sTotalStart;
    static JKRThreadSwitch_PreCallback  mUserPreCallback;
    static JKRThreadSwitch_PostCallback mUserPostCallback;

private:
    JKRHeap* mHeap;
    bool     mSetNextHeap;
    u8       field_0x9[3];
    u32      field_0xC;
    u32      field_0x10;
    u8       field_0x14[4];
    s64      field_0x18;
    u32      field_0x20;
    u32      field_0x24;
};

// ---------------------------------------------------------------------------
// JKRTask
// ---------------------------------------------------------------------------
class JKRTask : public JKRThread {
public:
    virtual ~JKRTask();
    virtual void* run();

    int check();

    static JSUList<JKRTask> sTaskList;
    static u8               sEndMesgQueue[32];

    JSULink<JKRTask>  mTaskLink;
    u8                field_0x8c[0x94 - 0x8C];
    OSMessageQueue*   field_0x94;
};

// ---------------------------------------------------------------------------
