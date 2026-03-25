/**
 * port/src/JSystem/JKernel/JKRThread.cpp
 * PC port implementation of JKRThread using std::thread.
 *
 * Key design decisions:
 *  - OSThread* wrapper constructor (wrapping an existing GC thread) simply records the
 *    pointer; no real thread is spawned.
 *  - The two allocating constructors allocate a stack buffer, initialise the
 *    OSMessageQueue, and launch a std::thread that calls the virtual run().
 *  - JKRSetCurrentHeap / JKRGetCurrentHeap use thread_local storage.
 */
#include "JSystem/JKernel/JKRThread.h"
#include "dolphin/os/OSMessage.h"
#include <cstdlib>
#include <cstring>
#include <new>

// ---------------------------------------------------------------------------
// Static member definitions
// ---------------------------------------------------------------------------
JSUList<JKRThread>  JKRThread::sThreadList;
void*               JKRIdleThread::sThread = nullptr;

JKRThreadSwitch*    JKRThreadSwitch::sManager           = nullptr;
u32                 JKRThreadSwitch::sTotalCount         = 0;
u64                 JKRThreadSwitch::sTotalStart         = 0;
JKRThreadSwitch_PreCallback  JKRThreadSwitch::mUserPreCallback  = nullptr;
JKRThreadSwitch_PostCallback JKRThreadSwitch::mUserPostCallback = nullptr;

JSUList<JKRTask>    JKRTask::sTaskList;
u8                  JKRTask::sEndMesgQueue[32] = {};

// ---------------------------------------------------------------------------
// Thread-local current heap
// ---------------------------------------------------------------------------
static thread_local JKRHeap* tl_currentHeap = nullptr;

JKRHeap* JKRSetCurrentHeap(JKRHeap* heap) {
    JKRHeap* prev = tl_currentHeap ? tl_currentHeap : JKRHeap::sCurrentHeap;
    tl_currentHeap = heap;
    return prev;
}

JKRHeap* JKRGetCurrentHeap() {
    return tl_currentHeap ? tl_currentHeap : JKRHeap::sCurrentHeap;
}

// ---------------------------------------------------------------------------
// Internal thread entry trampoline
// ---------------------------------------------------------------------------
static void* thread_trampoline(JKRThread* self) {
    // Set the thread's heap as current on this std::thread
    JKRSetCurrentHeap(self->getCurrentHeap());
    return self->run();
}

// ---------------------------------------------------------------------------
// setCommon_mesgQueue — allocate & initialise the message queue
// ---------------------------------------------------------------------------
void JKRThread::setCommon_mesgQueue(JKRHeap* heap, int msg_count) {
    mMessageCount = msg_count;
    if (msg_count > 0) {
        u32 buf_size = static_cast<u32>(msg_count) * sizeof(OSMessage);
        mMesgBuffer = static_cast<OSMessage*>(
            heap ? heap->alloc(buf_size, 4) : ::malloc(buf_size));
    } else {
        mMesgBuffer = nullptr;
    }
    OSInitMessageQueue(&mMessageQueue, mMesgBuffer, mMessageCount);
}

// ---------------------------------------------------------------------------
// setCommon_heapSpecified — allocate stack, create OSThread placeholder
// ---------------------------------------------------------------------------
void JKRThread::setCommon_heapSpecified(JKRHeap* heap, u32 stack_size, int /*prio*/) {
    mHeap = heap;
    mStackSize = stack_size;
    if (stack_size > 0) {
        mStackMemory = heap ? heap->alloc(stack_size, 32) : ::malloc(stack_size);
    } else {
        mStackMemory = nullptr;
    }
    // Allocate a placeholder OSThread struct (game code may read it)
    mThreadRecord = static_cast<OSThread*>(
        heap ? heap->alloc(sizeof(OSThread), 32) : ::malloc(sizeof(OSThread)));
    std::memset(mThreadRecord, 0, sizeof(OSThread));
}

// ---------------------------------------------------------------------------
// Constructor 1: allocates stack, creates thread
// ---------------------------------------------------------------------------
JKRThread::JKRThread(u32 stack_size, int msg_count, int prio)
    : JKRDisposer()
    , mThreadListLink(this)
    , mHeap(JKRHeap::sCurrentHeap)
    , mThreadRecord(nullptr)
    , mMesgBuffer(nullptr)
    , mMessageCount(0)
    , mStackMemory(nullptr)
    , mStackSize(0)
    , mCurrentHeap(JKRHeap::sCurrentHeap)
    , mCurrentHeapError(0)
    , _thread(nullptr)
    , _running(false)
{
    setCommon_heapSpecified(mHeap, stack_size, prio);
    setCommon_mesgQueue(mHeap, msg_count);
    sThreadList.append(&mThreadListLink);
}

// ---------------------------------------------------------------------------
// Constructor 2: allocates stack on the provided heap
// ---------------------------------------------------------------------------
JKRThread::JKRThread(JKRHeap* heap, u32 stack_size, int msg_count, int prio)
    : JKRDisposer()
    , mThreadListLink(this)
    , mHeap(heap ? heap : JKRHeap::sCurrentHeap)
    , mThreadRecord(nullptr)
    , mMesgBuffer(nullptr)
    , mMessageCount(0)
    , mStackMemory(nullptr)
    , mStackSize(0)
    , mCurrentHeap(heap ? heap : JKRHeap::sCurrentHeap)
    , mCurrentHeapError(0)
    , _thread(nullptr)
    , _running(false)
{
    setCommon_heapSpecified(mHeap, stack_size, prio);
    setCommon_mesgQueue(mHeap, msg_count);
    sThreadList.append(&mThreadListLink);
}

// ---------------------------------------------------------------------------
// Constructor 3: wraps an existing GC OSThread — no new thread on PC
// ---------------------------------------------------------------------------
JKRThread::JKRThread(OSThread* thread, int msg_count)
    : JKRDisposer()
    , mThreadListLink(this)
    , mHeap(JKRHeap::sCurrentHeap)
    , mThreadRecord(thread)
    , mMesgBuffer(nullptr)
    , mMessageCount(0)
    , mStackMemory(nullptr)
    , mStackSize(0)
    , mCurrentHeap(JKRHeap::sCurrentHeap)
    , mCurrentHeapError(0)
    , _thread(nullptr)
    , _running(false)
{
    setCommon_mesgQueue(mHeap, msg_count);
    sThreadList.append(&mThreadListLink);
}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
JKRThread::~JKRThread() {
    _running = false;
    if (_thread) {
        if (_thread->joinable()) _thread->detach();
        delete _thread;
        _thread = nullptr;
    }
    sThreadList.remove(&mThreadListLink);
    if (mMesgBuffer) { ::free(mMesgBuffer); mMesgBuffer = nullptr; }
    if (mStackMemory) { ::free(mStackMemory); mStackMemory = nullptr; }
    if (mThreadRecord) { ::free(mThreadRecord); mThreadRecord = nullptr; }
}

// ---------------------------------------------------------------------------
// start() — static trampoline called by std::thread
// ---------------------------------------------------------------------------
void* JKRThread::start(void* param) {
    JKRThread* self = static_cast<JKRThread*>(param);
    self->_running = true;
    return thread_trampoline(self);
}

// ---------------------------------------------------------------------------
// searchThread — not meaningful on PC
// ---------------------------------------------------------------------------
JKRThread* JKRThread::searchThread(OSThread* /*thread*/) {
    return nullptr;
}

// ---------------------------------------------------------------------------
// JKRThreadSwitch stubs
// ---------------------------------------------------------------------------
JKRThreadSwitch::JKRThreadSwitch(JKRHeap* heap)
    : mHeap(heap), mSetNextHeap(false)
    , field_0xC(0), field_0x10(0), field_0x18(0), field_0x20(0), field_0x24(0)
{
    std::memset(field_0x14, 0, sizeof(field_0x14));
}

void JKRThreadSwitch::draw(JKRThreadName_* /*names*/, JUTConsole* /*console*/) {}

JKRThreadSwitch* JKRThreadSwitch::createManager(JKRHeap* heap) {
    if (sManager) return sManager;
    void* buf = heap ? heap->alloc(sizeof(JKRThreadSwitch), 4) : ::malloc(sizeof(JKRThreadSwitch));
    sManager = new(buf) JKRThreadSwitch(heap);
    return sManager;
}

JKRThread* JKRThreadSwitch::enter(JKRThread* t, int /*param*/) { return t; }
void       JKRThreadSwitch::callback(OSThread* /*cur*/, OSThread* /*nxt*/) {}

// ---------------------------------------------------------------------------
// JKRTask stubs
// ---------------------------------------------------------------------------
JKRTask::~JKRTask() {}
void* JKRTask::run() { return nullptr; }
int   JKRTask::check() { return 0; }
