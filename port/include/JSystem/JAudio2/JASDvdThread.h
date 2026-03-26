#pragma once
#ifndef JASDVDTHREAD_H
#define JASDVDTHREAD_H

#include <types.h>
#include "dolphin/os/OSThread.h"

class JASTaskThread {
public:
    void pause(bool) {}
    OSThread* getThreadRecord() { return nullptr; }
    void sendCmdMsg(void (*cb)(void*), void* data, int) {
        if (cb != nullptr) {
            cb(data);
        }
    }
};

class JASDvd {
public:
    static JASTaskThread* getThreadPointer() { return nullptr; }
    static bool createThread(s32, int, u32) { return true; }
};

#endif /* JASDVDTHREAD_H */
