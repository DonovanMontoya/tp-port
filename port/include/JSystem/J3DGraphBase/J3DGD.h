#pragma once
// port stub — J3DGD.h: GC hardware FIFO write macros → no-ops on PC
#ifndef J3DGD_H
#define J3DGD_H
#include "dolphin/gx.h"
static inline void J3DGDWrite_u8(u8)  {}
static inline void J3DGDWrite_u16(u16) {}
static inline void J3DGDWrite_u32(u32) {}
static inline void J3DGDWrite_f32(f32) {}
#endif
