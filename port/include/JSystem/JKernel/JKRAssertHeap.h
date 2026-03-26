#pragma once
/**
 * port/include/JSystem/JKernel/JKRAssertHeap.h
 * PC port stub — JKRAssertHeap is a debug-checking heap wrapper.
 * On PC we just alias it to JKRExpHeap.
 */
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRThread.h"

class JKRAssertHeap : public JKRExpHeap {
public:
    JKRAssertHeap(u32 size, JKRHeap* parent, bool errFlag)
        : JKRExpHeap(size, parent, errFlag) {}
    virtual ~JKRAssertHeap() {}

    static JKRAssertHeap* create(u32 size, JKRHeap* parent, bool errFlag) {
        return new JKRAssertHeap(size, parent, errFlag);
    }
    static JKRAssertHeap* create(JKRHeap* parent) {
        return create(0x10000, parent, false);
    }

    void dump() const {}
};
