#pragma once
/**
 * port/include/JSystem/JKernel/JKRAram.h
 * PC port stub — GameCube ARAM does not exist on x86.
 * All methods are stubs returning nullptr/0.
 */
#include "JSystem/JKernel/JKRAramHeap.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "JSystem/JKernel/JKRThread.h"

class JKRHeap;
class JKRAMCommand;
class JKRAramBlock;

class JKRAram : public JKRThread {
private:
    JKRAram(u32 /*audio*/, u32 /*graph*/, s32 /*prio*/)
        : JKRThread(static_cast<OSThread*>(nullptr), 0)
        , mAudioMemoryPtr(0), mAudioMemorySize(0)
        , mGraphMemoryPtr(0), mGraphMemorySize(0)
        , mAramMemoryPtr(0),  mAramMemorySize(0)
        , mAramHeap(nullptr)
    {}
    virtual ~JKRAram() {}

    virtual void* run() override { return nullptr; }

public:
    u32 getAudioMemory()  const { return mAudioMemoryPtr; }
    u32 getAudioMemSize() const { return mAudioMemorySize; }
    u32 getGraphMemory()  const { return mGraphMemoryPtr; }
    u32 getGraphMemSize() const { return mGraphMemorySize; }

    u32          mAudioMemoryPtr;
    u32          mAudioMemorySize;
    u32          mGraphMemoryPtr;
    u32          mGraphMemorySize;
    u32          mAramMemoryPtr;
    u32          mAramMemorySize;
    JKRAramHeap* mAramHeap;
    u32          mStackArray[3];

public:
    // Factory / static operations — all no-ops on PC
    static JKRAram* create(u32 /*audio*/, u32 /*graph*/, s32 /*prio*/, s32, s32) {
        return nullptr;
    }
    static void checkOkAddress(u8*, u32, JKRAramBlock*, u32) {}
    static void changeGroupIdIfNeed(u8*, int) {}

    static JKRAramBlock* mainRamToAram(u8*, u32, u32, JKRExpandSwitch, u32, JKRHeap*, int, u32*) {
        return nullptr;
    }
    static u8* aramToMainRam(u32, u8* dst, u32, JKRExpandSwitch, u32, JKRHeap*, int, u32*) {
        return dst;
    }
    static void dump() {}

    static JKRAram*     getManager()   { return sAramObject; }
    static JKRAramHeap* getAramHeap()  { return nullptr; }
    static JSUList<JKRAMCommand>& getCommandList() { return sAramCommandList; }

    static u8 decideAramGroupId(int /*groupId*/) { return 0; }

    static u32  getSZSBufferSize()          { return sSZSBufferSize; }
    static void setSZSBufferSize(u32 size)  { sSZSBufferSize = size; }

    static OSMessageQueue     sMessageQueue;

private:
    static JKRAram*           sAramObject;
    static u32                sSZSBufferSize;
    static OSMessage          sMessageBuffer[4];
    static JSUList<JKRAMCommand> sAramCommandList;
};

// ---------------------------------------------------------------------------
// Inline helpers that the game uses directly
// ---------------------------------------------------------------------------
inline JKRAramBlock* JKRAllocFromAram(u32 /*size*/, JKRAramHeap::EAllocMode /*mode*/) {
    return nullptr;
}
inline void JKRFreeToAram(JKRAramBlock* /*block*/) {}

inline u8* JKRAramToMainRam(u32 p1, u8* p2, u32 p3, JKRExpandSwitch p4, u32 p5,
                             JKRHeap* p6, int p7, u32* p8) {
    return JKRAram::aramToMainRam(p1, p2, p3, p4, p5, p6, p7, p8);
}
inline JKRAramBlock* JKRMainRamToAram(u8* buf, u32 bufSize, u32 alignedSize,
                                       JKRExpandSwitch sw, u32 fileSize,
                                       JKRHeap* heap, int id, u32* pSize) {
    return JKRAram::mainRamToAram(buf, bufSize, alignedSize, sw, fileSize, heap, id, pSize);
}
inline u32 JKRGetAramTotalFreeSize() { return 0; }
inline u32 JKRGetAramFreeSize()      { return 0; }
inline u32 JKRGetAramUsedSize(u8 /*groupId*/) { return 0; }
