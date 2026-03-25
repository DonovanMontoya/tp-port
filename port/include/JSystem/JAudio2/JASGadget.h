#pragma once
/**
 * port/include/JSystem/JAudio2/JASGadget.h
 * PC port stub — defines JASGlobalInstance, JASPtrTable, JASPtrArray.
 * Blocks the real GC JASGadget.h from being included.
 */
#ifndef JASGADGET_H
#define JASGADGET_H

#include "port/types.h"
#include <cstring>

#define JAS_GLOBAL_INSTANCE_INIT {}

// ---------------------------------------------------------------------------
// JASGlobalInstance — singleton holder
// Compatible with both the GC calling convention (bool arg) and port usage.
// ---------------------------------------------------------------------------
template <typename T>
class JASGlobalInstance {
public:
    JASGlobalInstance(T* inst) { sInstance = inst; }
    JASGlobalInstance(bool setInstance) {
        if (setInstance) { sInstance = static_cast<T*>(this); }
    }
    ~JASGlobalInstance() {
        if (sInstance == static_cast<T*>(this)) { sInstance = nullptr; }
    }
    static T* getInstance() { return sInstance; }
    static void setInstance(T* p) { sInstance = p; }
    static T* sInstance;
};
template <typename T> T* JASGlobalInstance<T>::sInstance = nullptr;

// ---------------------------------------------------------------------------
// JASPtrTable — array of pointers with bounds-checked get/set
// ---------------------------------------------------------------------------
template <class T>
class JASPtrTable {
public:
    JASPtrTable(T** table, u32 size) : mTable(table), mSize(size) {
        memset(mTable, 0, size * sizeof(T*));
    }
    T* get(u32 index) const {
        if (index >= mSize) return nullptr;
        return mTable[index];
    }
    void set(u32 index, T* value) {
        if (index < mSize) mTable[index] = value;
    }
private:
    T**  mTable;
    u32  mSize;
};

// ---------------------------------------------------------------------------
// JASPtrArray — fixed-size specialisation of JASPtrTable
// ---------------------------------------------------------------------------
template <class T, size_t N>
class JASPtrArray : public JASPtrTable<T> {
public:
    JASPtrArray() : JASPtrTable<T>(mArray, N) {}
private:
    T* mArray[N];
};

#endif /* JASGADGET_H */
