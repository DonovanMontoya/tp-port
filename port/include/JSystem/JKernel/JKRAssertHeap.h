#pragma once
/**
 * port/include/JSystem/JKernel/JKRAssertHeap.h
 * PC port stub — JKRAssertHeap is a debug-checking heap wrapper.
 * On PC we just alias it to JKRExpHeap.
 */
#include "JSystem/JKernel/JKRHeap.h"

class JKRAssertHeap : public JKRExpHeap {
public:
    JKRAssertHeap(u32 size, JKRHeap* parent, bool errFlag)
        : JKRExpHeap(size, parent, errFlag) {}
    virtual ~JKRAssertHeap() {}

    static JKRAssertHeap* create(u32 size, JKRHeap* parent, bool errFlag) {
        return new JKRAssertHeap(size, parent, errFlag);
    }

    void dump() const {}
};

// mDoExt_getAssertHeap() — referenced by some debug code
inline JKRHeap* mDoExt_getAssertHeap() {
    return JKRHeap::sCurrentHeap;
}
