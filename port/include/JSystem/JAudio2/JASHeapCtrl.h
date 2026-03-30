#pragma once
/**
 * port/include/JSystem/JAudio2/JASHeapCtrl.h
 * PC port stub — JAS audio heap management (all no-ops/stubs).
 */
#ifndef JASHEAPCTRL_H
#define JASHEAPCTRL_H

#include "port/types.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <new>

struct JASDisposer {};
class JASHeap;

namespace JASThreadingModel {
template <typename T>
struct InterruptsDisable {
    struct Lock {
        explicit Lock(T&) {}
    };
};

struct ObjectLevelLockable {
    struct Lock {
        explicit Lock(ObjectLevelLockable&) {}
    };
};

template <typename T>
struct SingleThreaded {
    struct Lock {
        explicit Lock(T&) {}
    };
};
}  // namespace JASThreadingModel

namespace JASKernel {
    inline JKRHeap* getSystemHeap() { return JKRHeap::sSystemHeap; }
    inline JASHeap* getAramHeap() { return nullptr; }
}

class JASHeap {
public:
    JASHeap(JASDisposer* = nullptr) {}
    virtual ~JASHeap() {}
    bool alloc(u32, void**) { return false; }
    void free(void*) {}
    void free() {}
    bool isAllocated() const { return false; }
    u32 getSize() const { return 0; }
    void* getBase() const { return nullptr; }
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

template <typename T>
class JASPoolAllocObject {
public:
    static void* operator new(size_t size) { return ::operator new(size); }
    static void operator delete(void* ptr) { ::operator delete(ptr); }
};

template <size_t ChunkSize, typename Lockable>
class JASMemChunkPool : public Lockable {
public:
    void* alloc(u32 = ChunkSize) { return nullptr; }
    void free(void*, u32 = ChunkSize) {}
};

#endif /* JASHEAPCTRL_H */
