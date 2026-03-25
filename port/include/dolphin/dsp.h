#pragma once
/**
 * PC stub for dolphin/dsp.h
 * The DSPTaskInfo struct is preserved in full because card.h embeds it.
 * All DSP functions are no-ops — there is no DSP on PC.
 */

#include "dolphin/os.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*DSPCallback)(void* task);
typedef struct STRUCT_DSP_TASK DSPTaskInfo;

typedef struct STRUCT_DSP_TASK {
    /* 0x00 */ volatile u32 state;
    /* 0x04 */ volatile u32 priority;
    /* 0x08 */ volatile u32 flags;
    /* 0x0C */ u16* iram_mmem_addr;
    /* 0x10 */ u32 iram_length;
    /* 0x14 */ u32 iram_addr;
    /* 0x18 */ u16* dram_mmem_addr;
    /* 0x1C */ u32 dram_length;
    /* 0x20 */ u32 dram_addr;
    /* 0x24 */ u16 dsp_init_vector;
    /* 0x26 */ u16 dsp_resume_vector;
    /* 0x28 */ DSPCallback init_cb;
    /* 0x2C */ DSPCallback res_cb;
    /* 0x30 */ DSPCallback done_cb;
    /* 0x34 */ DSPCallback req_cb;
    /* 0x38 */ DSPTaskInfo* next;
    /* 0x3C */ DSPTaskInfo* prev;
    /* 0x40 */ OSTime t_context;
    /* 0x48 */ OSTime t_task;
} DSPTaskInfo;

static inline u32  DSPCheckMailToDSP(void)              { return 0; }
static inline u32  DSPCheckMailFromDSP(void)            { return 0; }
static inline u32  DSPReadCPUToDSPMbox(void)            { return 0; }
static inline u32  DSPReadMailFromDSP(void)             { return 0; }
static inline void DSPSendMailToDSP(u32 mail)           { (void)mail; }
static inline void DSPAssertInt(void)                   {}
static inline void DSPInit(void)                        {}
static inline BOOL DSPCheckInit(void)                   { return TRUE; }
static inline void DSPReset(void)                       {}
static inline void DSPHalt(void)                        {}
static inline void DSPUnhalt(void)                      {}
static inline u32  DSPGetDMAStatus(void)                { return 0; }

static inline DSPTaskInfo* DSPAddTask(DSPTaskInfo* task) { (void)task; return task; }
static inline DSPTaskInfo* DSPCancelTask(DSPTaskInfo* task) { (void)task; return NULL; }
static inline DSPTaskInfo* DSPAssertTask(DSPTaskInfo* task) { (void)task; return task; }

static inline DSPTaskInfo* __DSPGetCurrentTask(void)    { return NULL; }

#ifdef __cplusplus
}
#endif
