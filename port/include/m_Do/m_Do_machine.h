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

#endif /* M_DO_M_DO_MACHINE_H */
