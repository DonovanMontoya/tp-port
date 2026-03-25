#pragma once
/**
 * PC stub for dolphin/os/OSContext.h
 * OSContext holds PPC register state for context switching.
 * On PC we track only the minimal fields used by game code.
 */
#include "port/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    // General-purpose registers r0-r31 (64 entries for hi/lo halves)
    u32 gpr[32];
    u32 cr, lr, ctr, xer;
    f64 fpr[32];
    u32 fpscr;
    u32 srr0, srr1;
    u16 mode;
    u16 state;
    u32 gqr[8];
    f64 psf[32];
} OSContext;

#define OS_CONTEXT_STATE_EXC  (1 << 0)
#define OS_CONTEXT_STATE_FPSAVED (1 << 1)

void OSInitContext(OSContext* ctx, void* (*func)(void*), void* stack);
void OSLoadContext(OSContext* ctx);
void OSSaveContext(OSContext* ctx);

#ifdef __cplusplus
}
#endif
