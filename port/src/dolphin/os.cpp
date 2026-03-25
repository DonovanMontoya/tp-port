/**
 * src/dolphin/os.cpp
 * PC implementation of the GameCube OS abstraction layer.
 *
 * Maps GC cooperative threads → std::thread, GC heap → malloc/free,
 * GC time → high-resolution PC clock, GC messages → condition variables.
 */

#include "port/port.h"
#include "dolphin/os.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <thread>

// -----------------------------------------------------------------------
// Arena (used by game's heap initialisation — ignored on PC)
// -----------------------------------------------------------------------
static uint8_t  sFakeArena[32 * 1024 * 1024];  // 32 MB dummy
static void*    sArenaLo = sFakeArena;
static void*    sArenaHi = sFakeArena + sizeof(sFakeArena);

volatile OSHeapHandle __OSCurrHeap = 0;

void OSInit(void) {
    tp::log::info("OSInit — PC port layer");
}

void* OSGetArenaLo(void) { return sArenaLo; }
void* OSGetArenaHi(void) { return sArenaHi; }
void  OSSetArenaLo(void* lo) { sArenaLo = lo; }
void  OSSetArenaHi(void* hi) { sArenaHi = hi; }

void* OSGetMEM1ArenaLo(void) { return sArenaLo; }
void* OSGetMEM1ArenaHi(void) { return sArenaHi; }
void  OSSetMEM1ArenaLo(void* lo) { sArenaLo = lo; }
void  OSSetMEM1ArenaHi(void* hi) { sArenaHi = hi; }
void* OSGetMEM2ArenaLo(void) { return nullptr; }
void* OSGetMEM2ArenaHi(void) { return nullptr; }
void  OSSetMEM2ArenaLo(void* /*lo*/) {}
void  OSSetMEM2ArenaHi(void* /*hi*/) {}

// -----------------------------------------------------------------------
// Reporting
// -----------------------------------------------------------------------
void OSReport(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fputs("[GC] ", stderr);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

void OSReport_Error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fputs("[GC ERR] ", stderr);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

void OSReport_Warning(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fputs("[GC WRN] ", stderr);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

void OSPanic(const char* file, int line, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[GC PANIC] %s:%d: ", file, line);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);
    abort();
}

void OSFatal(u32 /*fg*/, u32 /*bg*/, const char* msg) {
    fprintf(stderr, "[GC FATAL] %s\n", msg);
    abort();
}

// -----------------------------------------------------------------------
// Time — simulate GC time base (~40.5 MHz)
// -----------------------------------------------------------------------
static auto sStartTime = std::chrono::high_resolution_clock::now();

OSTime OSGetTime(void) {
    auto now = std::chrono::high_resolution_clock::now();
    auto ns  = std::chrono::duration_cast<std::chrono::nanoseconds>(now - sStartTime).count();
    // Convert nanoseconds → GC ticks (40.5 MHz)
    return static_cast<OSTime>(ns * OS_TIME_SPEED / 1000000000LL);
}

OSTick OSGetTick(void) {
    return static_cast<OSTick>(OSGetTime() & 0xFFFFFFFF);
}

// -----------------------------------------------------------------------
// Context (stubs — context switching not used directly on PC)
// -----------------------------------------------------------------------
void OSInitContext(OSContext* ctx, void* (*func)(void*), void* stack) {
    if (ctx) memset(ctx, 0, sizeof(*ctx));
    (void)func; (void)stack;
}
void OSLoadContext(OSContext* /*ctx*/) {}
void OSSaveContext(OSContext* /*ctx*/) {}

// -----------------------------------------------------------------------
// Thread queue helpers
// -----------------------------------------------------------------------
void OSInitThreadQueue(OSThreadQueue* queue) {
    if (queue) { queue->head = queue->tail = nullptr; }
}

static thread_local OSThread* tCurrentThread = nullptr;

OSThread* OSGetCurrentThread(void) { return tCurrentThread; }

// -----------------------------------------------------------------------
// Thread — maps GC cooperative threads to std::thread
// -----------------------------------------------------------------------
struct HostThread {
    std::thread         thread;
    std::mutex          mtx;
    std::condition_variable cv;
    bool                running = true;
    void*               retVal  = nullptr;
};

int OSCreateThread(OSThread* gcThread, void* (*func)(void*), void* param,
                   void* stack, u32 /*stackSize*/, OSPriority /*priority*/, u16 attr)
{
    if (!gcThread || !func) return 0;
    memset(gcThread, 0, sizeof(*gcThread));
    gcThread->state = OS_THREAD_STATE_READY;
    gcThread->attr  = attr;

    auto* ht = new HostThread();
    gcThread->_hostThread = ht;

    ht->thread = std::thread([gcThread, func, param]() {
        tCurrentThread = gcThread;
        gcThread->state = OS_THREAD_STATE_RUNNING;
        gcThread->val = func(param);
        gcThread->state = OS_THREAD_STATE_MORIBUND;
    });

    if (attr & OS_THREAD_ATTR_DETACH) {
        ht->thread.detach();
    }
    return 1;
}

void OSExitThread(void* val) {
    if (tCurrentThread) tCurrentThread->val = val;
    // Thread function returns naturally
}

int OSJoinThread(OSThread* thread, void* /*val*/) {
    if (!thread || !thread->_hostThread) return 0;
    auto* ht = static_cast<HostThread*>(thread->_hostThread);
    if (ht->thread.joinable()) ht->thread.join();
    return 1;
}

void OSDetachThread(OSThread* thread) {
    if (!thread || !thread->_hostThread) return;
    auto* ht = static_cast<HostThread*>(thread->_hostThread);
    if (ht->thread.joinable()) ht->thread.detach();
}

s32 OSSuspendThread(OSThread* /*thread*/) { return 0; }
s32 OSResumeThread(OSThread* /*thread*/)  { return 0; }
void OSYieldThread(void) { std::this_thread::yield(); }
void OSCancelThread(OSThread* /*thread*/) {}
void OSClearStack(u8 /*val*/) {}
BOOL OSIsThreadSuspended(OSThread* /*t*/) { return FALSE; }
BOOL OSIsThreadTerminated(OSThread* t)    { return t ? t->state == OS_THREAD_STATE_MORIBUND : TRUE; }
s32 OSEnableScheduler(void)  { return 0; }
s32 OSDisableScheduler(void) { return 0; }
s32 OSCheckActiveThreads(void) { return 1; }
int OSSetThreadPriority(OSThread* /*t*/, OSPriority /*p*/) { return 1; }
s32 OSGetThreadPriority(OSThread* /*t*/) { return 16; }
void OSSetThreadSpecific(s32 idx, void* ptr) {
    if (tCurrentThread && idx >= 0 && idx < OS_THREAD_SPECIFIC_MAX)
        tCurrentThread->specific[idx] = ptr;
}
void* OSGetThreadSpecific(s32 idx) {
    if (tCurrentThread && idx >= 0 && idx < OS_THREAD_SPECIFIC_MAX)
        return tCurrentThread->specific[idx];
    return nullptr;
}
OSSwitchThreadCallback OSSetSwitchThreadCallback(OSSwitchThreadCallback /*cb*/) { return nullptr; }
OSThread* OSSetIdleFunction(OSIdleFunction /*fn*/, void* /*p*/, void* /*s*/, u32 /*ss*/) { return nullptr; }
OSThread* OSGetIdleFunction(void) { return nullptr; }

// -----------------------------------------------------------------------
// Thread queue (sleep/wakeup — used by audio/DVD threads)
// -----------------------------------------------------------------------
void OSSleepThread(OSThreadQueue* /*queue*/) {
    // Real implementation would park the thread; for now yield
    std::this_thread::yield();
}
void OSWakeupThread(OSThreadQueue* /*queue*/) {}

// -----------------------------------------------------------------------
// Mutex — backed by std::mutex
// -----------------------------------------------------------------------
void OSInitMutex(OSMutex* m) {
    if (!m) return;
    OSInitThreadQueue(&m->queue);
    m->thread = nullptr;
    m->count  = 0;
    m->_mutex = new std::mutex();
}

void OSLockMutex(OSMutex* m) {
    if (!m || !m->_mutex) return;
    static_cast<std::mutex*>(m->_mutex)->lock();
    m->thread = tCurrentThread;
    m->count++;
}

void OSUnlockMutex(OSMutex* m) {
    if (!m || !m->_mutex) return;
    if (--m->count == 0) m->thread = nullptr;
    static_cast<std::mutex*>(m->_mutex)->unlock();
}

BOOL OSTryLockMutex(OSMutex* m) {
    if (!m || !m->_mutex) return FALSE;
    bool ok = static_cast<std::mutex*>(m->_mutex)->try_lock();
    if (ok) { m->thread = tCurrentThread; m->count++; }
    return ok ? TRUE : FALSE;
}

void OSInitCond(OSCond* c) {
    if (!c) return;
    OSInitThreadQueue(&c->queue);
    c->_cond = new std::condition_variable_any();
}

void OSWaitCond(OSCond* c, OSMutex* m) {
    if (!c || !m || !c->_cond || !m->_mutex) return;
    auto* cv  = static_cast<std::condition_variable_any*>(c->_cond);
    auto* mtx = static_cast<std::mutex*>(m->_mutex);
    cv->wait(*mtx);
}

void OSSignalCond(OSCond* c) {
    if (!c || !c->_cond) return;
    static_cast<std::condition_variable_any*>(c->_cond)->notify_all();
}

// -----------------------------------------------------------------------
// Message queues — simple bounded FIFO backed by mutex+condvar
// -----------------------------------------------------------------------
struct MsgQueuePC {
    std::mutex              mtx;
    std::condition_variable cvSend;
    std::condition_variable cvRecv;
};

void OSInitMessageQueue(OSMessageQueue* mq, void* msgArray, s32 msgCount) {
    if (!mq) return;
    OSInitThreadQueue(&mq->queueSend);
    OSInitThreadQueue(&mq->queueReceive);
    mq->msgArray   = msgArray;
    mq->msgCount   = msgCount;
    mq->firstIndex = 0;
    mq->usedCount  = 0;
    mq->_mutex     = new MsgQueuePC();
    mq->_condSend  = nullptr;  // stored inside MsgQueuePC
    mq->_condRecv  = nullptr;
}

int OSSendMessage(OSMessageQueue* mq, void* msg, s32 flags) {
    if (!mq || !mq->_mutex) return 0;
    auto* pc = static_cast<MsgQueuePC*>(mq->_mutex);
    std::unique_lock<std::mutex> lock(pc->mtx);
    if (flags == OS_MESSAGE_BLOCK) {
        pc->cvSend.wait(lock, [mq]{ return mq->usedCount < mq->msgCount; });
    } else if (mq->usedCount >= mq->msgCount) {
        return 0;
    }
    void** arr = static_cast<void**>(mq->msgArray);
    s32 idx = (mq->firstIndex + mq->usedCount) % mq->msgCount;
    arr[idx] = msg;
    mq->usedCount++;
    pc->cvRecv.notify_one();
    return 1;
}

int OSReceiveMessage(OSMessageQueue* mq, void* msg, s32 flags) {
    if (!mq || !mq->_mutex) return 0;
    auto* pc = static_cast<MsgQueuePC*>(mq->_mutex);
    std::unique_lock<std::mutex> lock(pc->mtx);
    if (flags == OS_MESSAGE_BLOCK) {
        pc->cvRecv.wait(lock, [mq]{ return mq->usedCount > 0; });
    } else if (mq->usedCount == 0) {
        return 0;
    }
    void** arr = static_cast<void**>(mq->msgArray);
    if (msg) memcpy(msg, &arr[mq->firstIndex], sizeof(void*));
    mq->firstIndex = (mq->firstIndex + 1) % mq->msgCount;
    mq->usedCount--;
    pc->cvSend.notify_one();
    return 1;
}

int OSJamMessage(OSMessageQueue* mq, void* msg, s32 flags) {
    if (!mq || !mq->_mutex) return 0;
    auto* pc = static_cast<MsgQueuePC*>(mq->_mutex);
    std::unique_lock<std::mutex> lock(pc->mtx);
    if (flags == OS_MESSAGE_BLOCK) {
        pc->cvSend.wait(lock, [mq]{ return mq->usedCount < mq->msgCount; });
    } else if (mq->usedCount >= mq->msgCount) {
        return 0;
    }
    void** arr = static_cast<void**>(mq->msgArray);
    mq->firstIndex = (mq->firstIndex - 1 + mq->msgCount) % mq->msgCount;
    arr[mq->firstIndex] = msg;
    mq->usedCount++;
    pc->cvRecv.notify_one();
    return 1;
}

// -----------------------------------------------------------------------
// Reset / power
// -----------------------------------------------------------------------
void OSRegisterResetFunction(OSResetCallback /*cb*/) {}
void OSUnregisterResetFunction(OSResetCallback /*cb*/) {}
void OSResetSystem(s32 /*reset*/, u32 /*renderFrame*/, BOOL /*bootDol*/) { exit(0); }
