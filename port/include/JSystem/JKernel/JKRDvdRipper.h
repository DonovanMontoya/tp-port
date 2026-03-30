#pragma once
/**
 * port/include/JSystem/JKernel/JKRDvdRipper.h
 * PC port stub — DVD I/O replaced by host filesystem.
 * All load methods return nullptr; statics are preserved for ABI compatibility.
 */
#include "JSystem/JKernel/JKRCompression.h"
#include "JSystem/JSupport/JSUList.h"
#include "port/types.h"

enum JKRExpandSwitch {
    EXPAND_SWITCH_UNKNOWN0 = 0,
    EXPAND_SWITCH_UNKNOWN1 = 1,
    EXPAND_SWITCH_UNKNOWN2 = 2,
};

struct SYaz0Header {
    u32 signature;
    u32 length;
};

class JKRDMCommand {
    JKRDMCommand();
    ~JKRDMCommand();
};

class JKRHeap;
class JKRDvdFile;

class JKRDvdRipper {
public:
    static JSUList<JKRDMCommand> sDvdAsyncList;
    static u32  sSZSBufferSize;
    static bool errorRetry;

    enum EAllocDirection {
        UNKNOWN_EALLOC_DIRECTION  = 0,
        ALLOC_DIRECTION_FORWARD   = 1,
        ALLOC_DIRECTION_BACKWARD  = 2,
    };

    static void setSZSBufferSize(u32 size) { sSZSBufferSize = size; }
    static u32  getSZSBufferSize()         { return sSZSBufferSize; }
    static bool isErrorRetry()             { return errorRetry; }

    static void* loadToMainRAM(char const*, u8*, JKRExpandSwitch, u32, JKRHeap*,
                               EAllocDirection, u32, JKRCompression*, u32*);
    static void* loadToMainRAM(s32, u8*, JKRExpandSwitch, u32, JKRHeap*,
                               EAllocDirection, u32, JKRCompression*, u32*);
    static void* loadToMainRAM(JKRDvdFile*, u8*, JKRExpandSwitch, u32, JKRHeap*,
                               EAllocDirection, u32, JKRCompression*, u32*);
};

// ---------------------------------------------------------------------------
// Inline convenience wrappers
// ---------------------------------------------------------------------------
inline void* JKRDvdToMainRam(JKRDvdFile* file, u8* dst, JKRExpandSwitch sw, u32 dstLen,
                              JKRHeap* heap, JKRDvdRipper::EAllocDirection dir,
                              u32 offset, JKRCompression* comp, u32* retSize) {
    return JKRDvdRipper::loadToMainRAM(file, dst, sw, dstLen, heap, dir, offset, comp, retSize);
}
inline void* JKRDvdToMainRam(s32 entry, u8* dst, JKRExpandSwitch sw, u32 dstLen,
                              JKRHeap* heap, JKRDvdRipper::EAllocDirection dir,
                              u32 offset, JKRCompression* comp, u32* retSize) {
    return JKRDvdRipper::loadToMainRAM(entry, dst, sw, dstLen, heap, dir, offset, comp, retSize);
}
inline void* JKRDvdToMainRam(const char* name, u8* dst, JKRExpandSwitch sw, u32 dstLen,
                              JKRHeap* heap, JKRDvdRipper::EAllocDirection dir,
                              u32 offset, JKRCompression* comp, u32* retSize) {
    return JKRDvdRipper::loadToMainRAM(name, dst, sw, dstLen, heap, dir, offset, comp, retSize);
}
