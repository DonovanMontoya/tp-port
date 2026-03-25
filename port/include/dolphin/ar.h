#pragma once
/**
 * PC stub for dolphin/ar.h
 * AudioRAM (ARAM) was a separate 16 MB DRAM bank on the GameCube, accessed
 * via DMA.  On PC memory is flat, so:
 *   - ARAlloc / ARFree map to malloc / free via a thin compatibility layer
 *     implemented in src/dolphin/ar.cpp.
 *   - DMA transfer functions (ARStartDMA, ARQ*) are no-ops because the
 *     source and destination are already in the same address space.
 *   - ARGetSize / ARGetBaseAddress return plausible constants for code
 *     that checks the available ARAM budget.
 */

#include "port/types.h"
#include <cstdlib>  // malloc / free

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
// DMA direction constants
// ---------------------------------------------------------------------------
#define ARAM_DIR_MRAM_TO_ARAM 0x00
#define ARAM_DIR_ARAM_TO_MRAM 0x01

#define ARQ_TYPE_MRAM_TO_ARAM ARAM_DIR_MRAM_TO_ARAM
#define ARQ_TYPE_ARAM_TO_MRAM ARAM_DIR_ARAM_TO_MRAM

#define ARQ_DMA_ALIGNMENT     32

#define ARQ_PRIORITY_LOW      0
#define ARQ_PRIORITY_HIGH     1

// Convenience wrappers from the original header
#define ARStartDMARead(mmem, aram, len)  ARStartDMA(ARAM_DIR_ARAM_TO_MRAM, mmem, aram, len)
#define ARStartDMAWrite(mmem, aram, len) ARStartDMA(ARAM_DIR_MRAM_TO_ARAM, mmem, aram, len)

// ---------------------------------------------------------------------------
// ARQRequest — kept for source compatibility
// ---------------------------------------------------------------------------
typedef void (*ARQCallback)(u32 pointerToARQRequest);

struct ARQRequest {
    /* 0x00 */ struct ARQRequest* next;
    /* 0x04 */ u32      owner;
    /* 0x08 */ u32      type;
    /* 0x0C */ u32      priority;
    /* 0x10 */ u32      source;
    /* 0x14 */ u32      dest;
    /* 0x18 */ u32      length;
    /* 0x1C */ ARQCallback callback;
};
typedef struct ARQRequest ARQRequest;

// ---------------------------------------------------------------------------
// AR (AudioRAM) functions
// On PC ARAlloc returns a pointer cast to u32; ARFree calls free on it.
// Callers that treat the returned value as an opaque ARAM address are safe
// as long as they always access memory through AR DMA helpers (which are
// no-ops that treat ARAM addresses as regular pointers on PC).
// ---------------------------------------------------------------------------
ARQCallback ARRegisterDMACallback(ARQCallback callback);

static inline u32  ARGetDMAStatus(void)   { return 0; }

static inline void ARStartDMA(u32 type, u32 mainmem_addr, u32 aram_addr, u32 length)
{
    // On PC, ARAM addresses are real pointers so both sides are already in
    // main memory — nothing to transfer.
    (void)type; (void)mainmem_addr; (void)aram_addr; (void)length;
}

// ARAlloc: returns a u32 that is actually a heap pointer.
// The implementation in src/dolphin/ar.cpp tracks allocations and enforces
// the 32-byte alignment required by ARQ_DMA_ALIGNMENT.
u32  ARAlloc(u32 length);
u32  ARFree(u32* length);   // frees the last allocated block; sets *length if non-NULL

static inline BOOL ARCheckInit(void)      { return TRUE; }

// ARInit sets up an internal free-list using the supplied index table on GC.
// On PC we just return a dummy base address (16 MB ARAM budget).
static inline u32  ARInit(u32* stack_index_addr, u32 num_entries)
{
    (void)stack_index_addr; (void)num_entries;
    return 16 * 1024 * 1024;  // 16 MB
}

static inline void ARReset(void)          {}
static inline void ARSetSize(void)        {}

// Base address: expose the 16 MB budget that GC hardware had.
static inline u32  ARGetBaseAddress(void)     { return 0; }
static inline u32  ARGetSize(void)            { return 16 * 1024 * 1024; }
static inline u32  ARGetInternalSize(void)    { return 16 * 1024 * 1024; }

static inline void ARClear(u32 flag)          { (void)flag; }

// ---------------------------------------------------------------------------
// ARQ (AudioRAM Queue) functions — no-ops on PC
// ---------------------------------------------------------------------------
static inline void ARQInit(void)          {}
static inline void ARQReset(void)         {}

static inline void ARQPostRequest(ARQRequest* request, u32 owner, u32 type,
                                  u32 priority, u32 source, u32 dest,
                                  u32 length, ARQCallback callback)
{
    (void)request; (void)owner; (void)type; (void)priority;
    (void)source; (void)dest; (void)length;
    // Fire callback immediately since the "transfer" is instantaneous on PC.
    if (callback)
        callback((u32)(uintptr_t)request);
}

static inline void ARQRemoveRequest(ARQRequest* request)   { (void)request; }
static inline void ARQRemoveOwnerRequest(u32 owner)        { (void)owner; }
static inline void ARQFlushQueue(void)                     {}
static inline void ARQSetChunkSize(u32 size)               { (void)size; }
static inline u32  ARQGetChunkSize(void)                   { return 0x4000; }
static inline BOOL ARQCheckInit(void)                      { return TRUE; }

static inline u16  __ARGetInterruptStatus(void)            { return 0; }
static inline void __ARClearInterrupt(void)                {}

#ifdef __cplusplus
}
#endif
