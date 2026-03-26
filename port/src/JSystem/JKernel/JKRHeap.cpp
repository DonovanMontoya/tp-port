/**
 * src/JSystem/JKernel/JKRHeap.cpp
 * PC port stub for JKRHeap static members.
 */
#include "JSystem/JKernel/JKRHeap.h"

// Zero-initialize the static pointers first (constant initialization),
// then set them up via a lazy accessor to avoid SIOF.
JKRHeap* JKRHeap::sSystemHeap  = nullptr;
JKRHeap* JKRHeap::sCurrentHeap = nullptr;
JKRHeap* JKRHeap::sRootHeap2   = nullptr;

// Allocate the root heap on first use so static init order is irrelevant.
static JKRExpHeap* getOrCreateRootHeap() {
    static JKRExpHeap* s_root = nullptr;
    if (!s_root) {
        // Use placement new into a static buffer to avoid heap-before-heap issues
        alignas(JKRExpHeap) static char s_buf[sizeof(JKRExpHeap)];
        // Temporarily suppress JKRDisposer registering with a null heap
        s_root = new(s_buf) JKRExpHeap(0, nullptr, false);
        JKRHeap::sSystemHeap  = s_root;
        JKRHeap::sCurrentHeap = s_root;
        JKRHeap::sRootHeap2   = s_root;
    }
    return s_root;
}

// Force initialization before main via a constructor-priority global
namespace {
struct RootHeapInit {
    RootHeapInit() { getOrCreateRootHeap(); }
} g_rootHeapInit;
}
