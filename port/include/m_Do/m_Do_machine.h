#pragma once
/**
 * port/include/m_Do/m_Do_machine.h
 * PC port stub — machine/hardware-level init helpers.
 * Shadows the original which includes <gx.h> and GXRenderModeObj.
 * mDoMch_HeapCheckAll/mDoMch_Create are already stubbed in m_Do_stubs.cpp.
 */
#ifndef M_DO_M_DO_MACHINE_H
#define M_DO_M_DO_MACHINE_H

#include "port/types.h"

class JKRHeap;

BOOL mDoMch_IsProgressiveMode();
void mDoMch_HeapCheckAll();
void mDoMch_HeapFreeFillAll();
int  mDoMch_Create();

// GXRenderModeObj is defined in dolphin/gx/GXStruct.h (via gx.h)
#include "dolphin/gx/GXStruct.h"
extern GXRenderModeObj g_ntscZeldaProg;

class mDoMch_render_c {
public:
    static void setRenderModeObj(GXRenderModeObj* obj) { mRenderModeObj = obj; }
    static void setProgressiveMode() { setRenderModeObj(&g_ntscZeldaProg); }
    static u16  getEfbHeight()  { return mRenderModeObj ? mRenderModeObj->efbHeight : 480; }
    static u16  getFbWidth()    { return mRenderModeObj ? mRenderModeObj->fbWidth   : 640; }
    static GXRenderModeObj* getRenderModeObj() { return mRenderModeObj; }
    static GXRenderModeObj* mRenderModeObj;
};

namespace mDoMch {
    extern u8 mDebugFill;
    extern u8 mDebugFillNotUse;
}

#endif /* M_DO_M_DO_MACHINE_H */
