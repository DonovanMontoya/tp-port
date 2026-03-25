#pragma once
/**
 * port/include/JSystem/JAudio2/JASHeapCtrl.h
 * PC port stub — JAS audio heap management (all no-ops/stubs).
 */
#ifndef JASHEAPCTRL_H
#define JASHEAPCTRL_H

#include "port/types.h"
#include "JSystem/JKernel/JKRHeap.h"

struct JASDisposer {};

namespace JASKernel {
    inline JKRHeap* getSystemHeap() { return JKRHeap::sSystemHeap; }
}

class JASHeap {
public:
    JASHeap(JASDisposer* = nullptr) {}
    virtual ~JASHeap() {}
    bool alloc(u32, void**) { return false; }
    void free(void*) {}
    u32  getFreeSize() const { return 0x10000000u; }
};

class JASHeapCtrl {
public:
    JASHeapCtrl() {}
    virtual ~JASHeapCtrl() {}
    bool alloc(u32, void**) { return false; }
    void free(void*) {}
    JKRHeap* getSystemHeap() const { return JKRHeap::sSystemHeap; }
};

#endif /* JASHEAPCTRL_H */
