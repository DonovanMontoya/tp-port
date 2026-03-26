#pragma once
/**
 * port/include/m_Do/m_Do_ext.h
 * PC port stub — m_Do extension helpers.
 * Shadows the original which pulls in J3DAnimation, Z2SoundObject, etc.
 * f_op_overlap.cpp includes this header.
 */
#ifndef M_DO_M_DO_EXT_H
#define M_DO_M_DO_EXT_H

#include "port/types.h"
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "dolphin/os/OSThread.h"

class JKRArchive;
class JKRAssertHeap;
class JKRExpHeap;
class JKRSolidHeap;
class JUTFont;
struct ResTIMG;
class J3DModel;
class J3DModelData;

struct J3DTransformInfo;

// Animation stub base
class mDoExt_baseAnm {
public:
    void* mpAnmBase = nullptr;
    f32   mFrame    = 0.0f;
    f32   mSpeed    = 1.0f;
    bool  mIsLoop   = false;
};

// BCK — bone colour key animation
class mDoExt_bckAnm : public mDoExt_baseAnm {
public:
    void* mAnm = nullptr;
    mDoExt_bckAnm() {}
};

// BPK — bone path key
class mDoExt_bpkAnm : public mDoExt_baseAnm {
public:
    mDoExt_bpkAnm() {}
};

// BRK — register key
class mDoExt_brkAnm : public mDoExt_baseAnm {
public:
    mDoExt_brkAnm() {}
};

// BTK — texture key
class mDoExt_btkAnm : public mDoExt_baseAnm {
public:
    mDoExt_btkAnm() {}
};

// Morph callback base classes
class mDoExt_McaMorfCallBack1_c {
public:
    virtual int execute(u16, J3DTransformInfo*) { return 1; }
    virtual ~mDoExt_McaMorfCallBack1_c() {}
};
class mDoExt_McaMorfCallBack2_c {
public:
    virtual int execute(u16, J3DTransformInfo*) { return 1; }
    virtual ~mDoExt_McaMorfCallBack2_c() {}
};

class DummyCheckHeap : public JKRDisposer {
public:
    DummyCheckHeap();
    ~DummyCheckHeap();
    int isVirgin();
    JKRHeap* getHeap();
    void setHeap(JKRHeap*);

    void* mAlloc;
    int mSize;
    JKRHeap* mHeap;
};

namespace mDoExt {

inline JKRExpHeap* getHeap(int /*tag*/) { return nullptr; }

} // namespace mDoExt

OSThread* mDoExt_GetCurrentRunningThread();
extern JKRExpHeap* zeldaHeap;
extern JKRExpHeap* gameHeap;
extern JKRExpHeap* archiveHeap;
extern JKRExpHeap* commandHeap;
extern JKRExpHeap* dbPrintHeap;
extern JKRAssertHeap* assertHeap;
extern DummyCheckHeap* dch;
JKRExpHeap* mDoExt_getZeldaHeap();
JKRExpHeap* mDoExt_getGameHeap();
JKRExpHeap* mDoExt_getArchiveHeap();
JKRExpHeap* mDoExt_getArchiveHeapPtr();
JKRExpHeap* mDoExt_getCommandHeap();
JKRExpHeap* mDoExt_getDbPrintHeap();
JKRExpHeap* mDoExt_getJ2dHeap();
JKRAssertHeap* mDoExt_getAssertHeap();
JKRExpHeap* mDoExt_createDbPrintHeap(u32 heapSize, JKRHeap* parentHeap);
JKRAssertHeap* mDoExt_createAssertHeap(JKRHeap* parentHeap);
JKRExpHeap* mDoExt_createCommandHeap(u32 heapSize, JKRHeap* parentHeap);
JKRExpHeap* mDoExt_createArchiveHeap(u32 heapSize, JKRHeap* parentHeap);
JKRExpHeap* mDoExt_createJ2dHeap(u32 heapSize, JKRHeap* parentHeap);
JKRExpHeap* mDoExt_createZeldaHeap(u32 heapSize, JKRHeap* parentHeap);
JKRExpHeap* mDoExt_createGameHeap(u32 heapSize, JKRHeap* parentHeap);
JUTFont* mDoExt_getMesgFont();
JUTFont* mDoExt_getSubFont();
JUTFont* mDoExt_getRubyFont();
void mDoExt_setAraCacheSize(u32 size);

J3DModel* mDoExt_J3DModel__create(J3DModelData* md, u32 flags, u32 param);
void mDoExt_modelUpdateDL(J3DModel* model);
JKRSolidHeap* mDoExt_createSolidHeap(s32 size, JKRHeap* heap, u32 align);
JKRSolidHeap* mDoExt_createSolidHeapFromGame(u32 size, u32 align);
JKRSolidHeap* mDoExt_createSolidHeapFromGameToCurrent(u32 size, u32 align);
void mDoExt_destroySolidHeap(JKRSolidHeap* heap);
u32 mDoExt_adjustSolidHeap(JKRSolidHeap* heap);

#endif /* M_DO_M_DO_EXT_H */
