#pragma once
/**
 * port/include/JSystem/JKernel/JKRAramHeap.h
 * PC port stub — ARAM (GameCube auxiliary RAM) does not exist on x86.
 * All methods are no-ops returning safe defaults.
 */
#include "JSystem/JKernel/JKRDisposer.h"
#include "port/types.h"

// Forward declare block type (original is GC-specific)
class JKRAramBlock;

class JKRAramHeap : public JKRDisposer {
public:
    enum EAllocMode {
        HEAD = 0,
        TAIL = 1,
    };

    JKRAramHeap(u32 /*start*/, u32 /*size*/) {}
    virtual ~JKRAramHeap() {}

    // Allocation stubs — no ARAM on PC
    JKRAramBlock* alloc(u32 /*size*/, EAllocMode /*mode*/) { return nullptr; }
    JKRAramBlock* allocFromHead(u32 /*size*/)               { return nullptr; }
    JKRAramBlock* allocFromTail(u32 /*size*/)               { return nullptr; }
    void          free(JKRAramBlock* /*block*/)             {}

    // Size queries — report infinite free space
    u32 getFreeSize()      { return 0; }
    u32 getTotalFreeSize() { return 0; }
    u32 getUsedSize(u8 /*groupId*/) { return 0; }

    // Debug dump — no-op
    void dump() {}

    u8       getCurrentGroupID() const { return mGroupId; }
    JKRHeap* getMgrHeap()        const { return nullptr; }

    // Static list stub
    static JSUList<JKRAramBlock> sAramList;

private:
    u8 mGroupId = 0;
};
