#pragma once
/**
 * PC stub for dolphin/os/OSThread.h
 * Maps GC cooperative threads to host std::thread / POSIX primitives.
 *
 * The GC used a cooperative priority-based scheduler.  On PC we run
 * audio/DVD threads as real OS threads (see src/dolphin/os.cpp).
 */
#include "port/types.h"
#include "dolphin/os/OSContext.h"
#include <thread>
#include <atomic>

#ifdef __cplusplus
extern "C" {
#endif

typedef s32 OSPriority;

typedef struct OSThread OSThread;
typedef struct OSMutex  OSMutex;

typedef struct OSThreadQueue {
    OSThread* head;
    OSThread* tail;
} OSThreadQueue;

typedef struct OSThreadLink {
    OSThread* next;
    OSThread* prev;
} OSThreadLink;

typedef struct OSMutexQueue {
    OSMutex* head;
    OSMutex* tail;
} OSMutexQueue;

typedef struct OSMutexLink {
    OSMutex* next;
    OSMutex* prev;
} OSMutexLink;

struct OSThread {
    OSContext      context;
    u16            state;
    u16            attr;
    s32            suspend;
    OSPriority     priority;
    OSPriority     base;
    void*          val;
    OSThreadQueue* queue;
    OSThreadLink   link;
    OSThreadQueue  queueJoin;
    OSMutex*       mutex;
    OSMutexQueue   queueMutex;
    OSThreadLink   linkActive;
    u8*            stackBase;
    u32*           stackEnd;
    s32            error;
    void*          specific[2];
    // PC-only: backing host thread
    void*          _hostThread;
};

enum OS_THREAD_STATE {
    OS_THREAD_STATE_READY    = 1,
    OS_THREAD_STATE_RUNNING  = 2,
    OS_THREAD_STATE_WAITING  = 4,
    OS_THREAD_STATE_MORIBUND = 8,
};

#define OS_PRIORITY_MIN        0
#define OS_PRIORITY_MAX        31
#define OS_PRIORITY_IDLE       OS_PRIORITY_MAX
#define OS_THREAD_SPECIFIC_MAX 2
#define OS_THREAD_ATTR_DETACH  0x0001u
#define OS_THREAD_STACK_MAGIC  0xDEADBABE

typedef void (*OSSwitchThreadCallback)(OSThread*, OSThread*);
typedef void (*OSIdleFunction)(void*);

void  OSInitThreadQueue(OSThreadQueue* queue);
void  OSSleepThread(OSThreadQueue* queue);
void  OSWakeupThread(OSThreadQueue* queue);
s32   OSSuspendThread(OSThread* thread);
s32   OSResumeThread(OSThread* thread);
OSThread* OSGetCurrentThread(void);
s32   OSEnableScheduler(void);
s32   OSDisableScheduler(void);
void  OSCancelThread(OSThread* thread);
void  OSClearStack(u8 val);
BOOL  OSIsThreadSuspended(OSThread* thread);
BOOL  OSIsThreadTerminated(OSThread* thread);
void  OSYieldThread(void);
int   OSCreateThread(OSThread* thread, void* (*func)(void*), void* param,
                     void* stack, u32 stackSize, OSPriority priority, u16 attr);
void  OSExitThread(void* val);
int   OSJoinThread(OSThread* thread, void* val);
void  OSDetachThread(OSThread* thread);
int   OSSetThreadPriority(OSThread* thread, OSPriority priority);
s32   OSGetThreadPriority(OSThread* thread);
OSThread* OSSetIdleFunction(OSIdleFunction fn, void* param, void* stack, u32 stackSize);
OSThread* OSGetIdleFunction(void);
s32   OSCheckActiveThreads(void);
void  OSSetThreadSpecific(s32 index, void* ptr);
void* OSGetThreadSpecific(s32 index);
OSSwitchThreadCallback OSSetSwitchThreadCallback(OSSwitchThreadCallback cb);
static inline void* OSGetStackPointer(void) {
    return __builtin_frame_address(0);
}

#define IsSuspended(suspend) ((suspend) > 0)

#ifdef __cplusplus
}
#endif
