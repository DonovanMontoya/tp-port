#pragma once
/**
 * PC stub for dolphin/exi.h
 * Types and constants are kept identical to the original.
 * All EXI bus functions are no-ops or return FALSE/0 — there is no
 * external serial interface on PC.
 */

#include "dolphin/os.h"

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
// Device ID constants
// ---------------------------------------------------------------------------
#define EXI_MEMORY_CARD_59     0x00000004u
#define EXI_MEMORY_CARD_123    0x00000008u
#define EXI_MEMORY_CARD_251    0x00000010u
#define EXI_MEMORY_CARD_507    0x00000020u
#define EXI_MEMORY_CARD_1019   0x00000040u
#define EXI_MEMORY_CARD_2043   0x00000080u

#define EXI_MEMORY_CARD_1019A  0x00000140u
#define EXI_MEMORY_CARD_1019B  0x00000240u
#define EXI_MEMORY_CARD_1019C  0x00000340u
#define EXI_MEMORY_CARD_1019D  0x00000440u
#define EXI_MEMORY_CARD_1019E  0x00000540u
#define EXI_MEMORY_CARD_1019F  0x00000640u
#define EXI_MEMORY_CARD_1019G  0x00000740u

#define EXI_MEMORY_CARD_2043A  0x00000180u
#define EXI_MEMORY_CARD_2043B  0x00000280u
#define EXI_MEMORY_CARD_2043C  0x00000380u
#define EXI_MEMORY_CARD_2043D  0x00000480u
#define EXI_MEMORY_CARD_2043E  0x00000580u
#define EXI_MEMORY_CARD_2043F  0x00000680u
#define EXI_MEMORY_CARD_2043G  0x00000780u

#define EXI_USB_ADAPTER        0x01010000u
#define EXI_NPDP_GDEV          0x01020000u

#define EXI_MODEM              0x02020000u
#define EXI_ETHER              0x04020200u
#define EXI_MIC                0x04060000u
#define EXI_AD16               0x04120000u
#define EXI_RS232C             0x04040404u
#define EXI_ETHER_VIEWER       0x04220001u
#define EXI_STREAM_HANGER      0x04130000u
#define EXI_MARLIN             0x03010000u
#define EXI_IS_VIEWER          0x05070000u

// Transfer direction
#define EXI_READ  0
#define EXI_WRITE 1

// Clock frequency selectors
#define EXI_FREQ_1M  0
#define EXI_FREQ_2M  1
#define EXI_FREQ_4M  2
#define EXI_FREQ_8M  3
#define EXI_FREQ_16M 4
#define EXI_FREQ_32M 5

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------
typedef void (*EXICallback)(s32 chan, OSContext* context);

typedef struct EXIControl {
    EXICallback      exiCallback;
    EXICallback      tcCallback;
    EXICallback      extCallback;
    volatile u32     state;
    int              immLen;
    u8*              immBuf;
    u32              dev;
    u32              id;
    s32              idTime;
    int              items;
    struct {
        u32         dev;
        EXICallback callback;
    } queue[3];
} EXIControl;

// ---------------------------------------------------------------------------
// Function stubs
// ---------------------------------------------------------------------------
static inline EXICallback EXISetExiCallback(s32 channel, EXICallback callback)
{
    (void)channel;
    (void)callback;
    return NULL;
}

static inline void  EXIInit(void)                                    {}
static inline BOOL  EXILock(s32 ch, u32 dev, EXICallback cb)
                             { (void)ch; (void)dev; (void)cb; return TRUE; }
static inline BOOL  EXIUnlock(s32 ch)                               { (void)ch; return TRUE; }
static inline BOOL  EXISelect(s32 ch, u32 dev, u32 freq)
                             { (void)ch; (void)dev; (void)freq; return FALSE; }
static inline BOOL  EXIDeselect(s32 ch)                             { (void)ch; return FALSE; }
static inline BOOL  EXIImm(s32 ch, void* buf, s32 len, u32 type, EXICallback cb)
                             { (void)ch; (void)buf; (void)len; (void)type; (void)cb; return FALSE; }
static inline BOOL  EXIImmEx(s32 ch, void* buf, s32 len, u32 type)
                             { (void)ch; (void)buf; (void)len; (void)type; return FALSE; }
static inline BOOL  EXIDma(s32 ch, void* buf, s32 len, u32 type, EXICallback cb)
                             { (void)ch; (void)buf; (void)len; (void)type; (void)cb; return FALSE; }
static inline BOOL  EXISync(s32 ch)                                 { (void)ch; return FALSE; }
static inline BOOL  EXIProbe(s32 ch)                                { (void)ch; return FALSE; }
static inline s32   EXIProbeEx(s32 ch)                              { (void)ch; return -1; }
static inline BOOL  EXIAttach(s32 ch, EXICallback cb)
                             { (void)ch; (void)cb; return FALSE; }
static inline BOOL  EXIDetach(s32 ch)                               { (void)ch; return FALSE; }
static inline u32   EXIGetState(s32 ch)                             { (void)ch; return 0; }
static inline s32   EXIGetID(s32 ch, u32 dev, u32* id)
                             { (void)ch; (void)dev; if(id) *id = 0; return -1; }
static inline void  EXIProbeReset(void)                             {}
static inline int   EXISelectSD(s32 ch, u32 dev, u32 freq)
                             { (void)ch; (void)dev; (void)freq; return 0; }
static inline s32   EXIGetType(s32 ch, u32 dev, u32* type)
                             { (void)ch; (void)dev; if(type) *type = 0; return -1; }
static inline char* EXIGetTypeString(u32 type)                      { (void)type; return (char*)""; }

#ifdef __cplusplus
}
#endif
