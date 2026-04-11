#pragma once
// port stub — J3DFifo.h: GC FIFO command writes → no-ops on PC
#ifndef J3DFIFO_H
#define J3DFIFO_H
#include "dolphin/gx.h"
static inline void J3DFifoLoadBPCmd(u32) {}
static inline void J3DFifoWriteXFCmdHdr(u16, u8) {}
static inline void J3DFifoLoadXFCmdHdr(u16, u8) {}
static inline void J3DFifoLoadXFCmd(u16, u8, const u32*) {}
static inline void J3DFifoLoadCPCmd(u8, u32) {}
static inline void J3DFifoWriteCPCmd(u8, u32) {}
static inline void J3DFifoLoadIndx(u8, u16, u16) {}
static inline void J3DFifoWriteXFCmd(u16, u16) {}
static inline void J3DFifoLoadPosMtxIndx(u16, u32) {}
static inline void J3DFifoLoadNrmMtxIndx3x3(u16, u32) {}
static inline void J3DFifoWriteImm2(u8, u16) {}
static inline void J3DFifoWriteImm3(u8, u16, u16) {}
static inline void J3DFifoWriteImm4(u8, u16, u16, u16) {}
static inline void J3DFifoWriteInd(u8, u8, u32) {}
#endif
