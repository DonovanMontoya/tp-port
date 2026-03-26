#pragma once
/**
 * port/include/JSystem/JKernel/JKRMemArchive.h
 * PC port stub — in-memory RARC archive loader.
 * Class structure and vtable layout preserved; methods stubbed.
 */
#include "JSystem/JKernel/JKRArchive.h"
#include "port/types.h"

enum JKRMemBreakFlag {
    JKRMEMBREAK_FLAG_UNKNOWN0 = 0,
    JKRMEMBREAK_FLAG_UNKNOWN1 = 1,
};

class JKRMemArchive : public JKRArchive {
public:
    JKRMemArchive(s32 /*entryNum*/, JKRArchive::EMountDirection /*dir*/)
        : JKRArchive(), mArcHeader(nullptr), mArchiveData(nullptr), mIsOpen(true) {}

    JKRMemArchive(void* /*data*/, u32 /*size*/, JKRMemBreakFlag /*flag*/)
        : JKRArchive(), mArcHeader(nullptr), mArchiveData(nullptr), mIsOpen(true) {}

    virtual ~JKRMemArchive() {}

    bool open(s32 /*entryNum*/, JKRArchive::EMountDirection /*dir*/) { mIsOpen = true; return true; }
    bool open(void* /*data*/, u32 /*size*/, JKRMemBreakFlag /*flag*/) { mIsOpen = true; return true; }
    bool isMounted() const { return mIsOpen; }

    /* vt[09] */ virtual void removeResourceAll() override {}
    /* vt[10] */ virtual bool removeResource(void*) override { return false; }

    /* vt[15] */ virtual u32   getExpandedResSize(const void*) const override { return 0; }
    /* vt[16] */ virtual void* fetchResource(SDIFileEntry*, u32*) override { return nullptr; }
    /* vt[17] */ virtual void* fetchResource(void*, u32, SDIFileEntry*, u32*) override { return nullptr; }

    static u32 fetchResource_subroutine(u8*, u32, u8*, u32, JKRCompression) { return 0; }

    SArcHeader* getArcHeader() { return mArcHeader; }

    SArcHeader* mArcHeader;
    u8*         mArchiveData;
    bool        mIsOpen;
    u8          _pad[3];
};
