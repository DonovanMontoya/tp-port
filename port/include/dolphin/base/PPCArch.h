#pragma once
/**
 * PC stub for dolphin/base/PPCArch.h
 * Keeps all SPR register number defines and MSR/HID/GQR/FPSCR bit defines
 * exactly as the original.  Hardware-specific inline asm (mfspr/mtspr,
 * sync, eieio, dcbz, etc.) is replaced with no-op stubs so the code
 * compiles on x86 without modification.
 *
 * PPCHalt() calls abort() — matching the "halt the processor" intent.
 */

#include "port/types.h"
#include <cstdlib>  // abort

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
// Special-purpose register numbers (kept for source compatibility)
// ---------------------------------------------------------------------------
#define CTR     9
#define XER     1
#define LR      8

#define UPMC1   937
#define UPMC2   938
#define UPMC3   941
#define UPMC4   942

#define USIA    939

#define UMMCR0  936
#define UMMCR1  940

#define HID0    1008
#define HID1    1009

#define PVR     287

#define IBAT0U  528
#define IBAT0L  529
#define IBAT1U  530
#define IBAT1L  531
#define IBAT2U  532
#define IBAT2L  533
#define IBAT3U  534
#define IBAT3L  535

#define DBAT0U  536
#define DBAT0L  537
#define DBAT1U  538
#define DBAT1L  539
#define DBAT2U  540
#define DBAT2L  541
#define DBAT3U  542
#define DBAT3L  543

#define SDR1    25

#define SPRG0   272
#define SPRG1   273
#define SPRG2   274
#define SPRG3   275

#define DAR     19
#define DSISR   18

#define SRR0    26
#define SRR1    27

#define EAR     282

#define DABR    1013

#define TBL     284
#define TBU     285

#define L2CR    1017

#define DEC     22

#define IABR    1010

#define PMC1    953
#define PMC2    954
#define PMC3    957
#define PMC4    958

#define SIA     955

#define MMCR0   952
#define MMCR1   956

#define THRM1   1020
#define THRM2   1021
#define THRM3   1022

#define ICTC    1019

#define GQR0    912
#define GQR1    913
#define GQR2    914
#define GQR3    915
#define GQR4    916
#define GQR5    917
#define GQR6    918
#define GQR7    919

#define HID2    920

#define WPAR    921

#define DMA_U   922
#define DMA_L   923

// ---------------------------------------------------------------------------
// MSR bits
// ---------------------------------------------------------------------------
#define MSR_POW         0x00040000u
#define MSR_ILE         0x00010000u
#define MSR_EE          0x00008000u
#define MSR_PR          0x00004000u
#define MSR_FP          0x00002000u
#define MSR_ME          0x00001000u
#define MSR_FE0         0x00000800u
#define MSR_SE          0x00000400u
#define MSR_BE          0x00000200u
#define MSR_FE1         0x00000100u
#define MSR_IP          0x00000040u
#define MSR_IR          0x00000020u
#define MSR_DR          0x00000010u
#define MSR_PM          0x00000004u
#define MSR_RI          0x00000002u
#define MSR_LE          0x00000001u

#define MSR_POW_BIT     13
#define MSR_ILE_BIT     15
#define MSR_EE_BIT      16
#define MSR_PR_BIT      17
#define MSR_FP_BIT      18
#define MSR_ME_BIT      19
#define MSR_FE0_BIT     20
#define MSR_SE_BIT      21
#define MSR_BE_BIT      22
#define MSR_FE1_BIT     23
#define MSR_IP_BIT      25
#define MSR_IR_BIT      26
#define MSR_DR_BIT      27
#define MSR_PM_BIT      29
#define MSR_RI_BIT      30
#define MSR_LE_BIT      31

// ---------------------------------------------------------------------------
// HID0 bits
// ---------------------------------------------------------------------------
#define HID0_EMCP       0x80000000u
#define HID0_DBP        0x40000000u
#define HID0_EBA        0x20000000u
#define HID0_EBD        0x10000000u
#define HID0_BCLK       0x08000000u
#define HID0_ECLK       0x02000000u
#define HID0_PAR        0x01000000u
#define HID0_DOZE       0x00800000u
#define HID0_NAP        0x00400000u
#define HID0_SLEEP      0x00200000u
#define HID0_DPM        0x00100000u
#define HID0_NHR        0x00010000u
#define HID0_ICE        0x00008000u
#define HID0_DCE        0x00004000u
#define HID0_ILOCK      0x00002000u
#define HID0_DLOCK      0x00001000u
#define HID0_ICFI       0x00000800u
#define HID0_DCFI       0x00000400u
#define HID0_SPD        0x00000200u
#define HID0_IFEM       0x00000100u
#define HID0_SGE        0x00000080u
#define HID0_DCFA       0x00000040u
#define HID0_BTIC       0x00000020u
#define HID0_ABE        0x00000008u
#define HID0_BHT        0x00000004u
#define HID0_NOOPTI     0x00000001u

#define HID0_ICE_BIT    16
#define HID0_DCE_BIT    17
#define HID0_ILOCK_BIT  18
#define HID0_DLOCK_BIT  19

// ---------------------------------------------------------------------------
// HID2 bits
// ---------------------------------------------------------------------------
#define HID2_LSQE           0x80000000u
#define HID2_WPE            0x40000000u
#define HID2_PSE            0x20000000u
#define HID2_LCE            0x10000000u

#define HID2_DCHERR         0x00800000u
#define HID2_DNCERR         0x00400000u
#define HID2_DCMERR         0x00200000u
#define HID2_DQOERR         0x00100000u
#define HID2_DCHEE          0x00080000u
#define HID2_DNCEE          0x00040000u
#define HID2_DCMEE          0x00020000u
#define HID2_DQOEE          0x00010000u

#define HID2_DMAQL_MASK     0x0F000000u
#define HID2_DMAQL_SHIFT    24

#define HID2_LSQE_BIT       0
#define HID2_WPE_BIT        1
#define HID2_PSE_BIT        2
#define HID2_LCE_BIT        3

#define HID2_DCHERR_BIT     8
#define HID2_DNCERR_BIT     9
#define HID2_DCMERR_BIT     10
#define HID2_DQOERR_BIT     11
#define HID2_DCHEE_BIT      12
#define HID2_DNCEE_BIT      13
#define HID2_DCMEE_BIT      14
#define HID2_DQOEE_BIT      15

// ---------------------------------------------------------------------------
// GQR field masks
// ---------------------------------------------------------------------------
#define GQR_LOAD_SCALE_MASK         0x3F000000u
#define GQR_LOAD_TYPE_MASK          0x00070000u
#define GQR_STORE_SCALE_MASK        0x00003F00u
#define GQR_STORE_TYPE_MASK         0x00000007u

typedef struct {
    u32 _pad0       :2;
    u32 loadScale   :6;
    u32 _pad1       :5;
    u32 loadType    :3;
    u32 _pad2       :2;
    u32 storeScale  :6;
    u32 _pad3       :5;
    u32 storeType   :3;
} PPC_GQR_t;

typedef union {
    u32        val;
    PPC_GQR_t  f;
} PPC_GQR_u;

// ---------------------------------------------------------------------------
// DMA register masks
// ---------------------------------------------------------------------------
#define DMA_U_ADDR_MASK             0xFFFFFFE0u
#define DMA_U_LEN_U_MASK            0x0000001Fu

#define DMA_L_LC_ADDR_MASK          0xFFFFFFE0u
#define DMA_L_LOAD                  0x00000010u
#define DMA_L_STORE                 0x00000000u
#define DMA_L_LEN_MASK              0x0000000Cu
#define DMA_L_TRIGGER               0x00000002u
#define DMA_L_FLUSH                 0x00000001u

typedef struct {
    u32 memAddr  :27;
    u32 dmaLenU  :5;
} PPC_DMA_U_t;

typedef union {
    u32          val;
    PPC_DMA_U_t  f;
} PPC_DMA_U_u;

typedef struct {
    u32 lcAddr      :27;
    u32 dmaLd       :1;
    u32 dmaLenL     :2;
    u32 dmaTrigger  :1;
    u32 dmaFlush    :1;
} PPC_DMA_L_t;

typedef union {
    u32          val;
    PPC_DMA_L_t  f;
} PPC_DMA_L_u;

// ---------------------------------------------------------------------------
// WPAR / SRR1 / L2CR defines
// ---------------------------------------------------------------------------
#define WPAR_ADDR                   0xFFFFFFE0u
#define WPAR_BNE                    0x00000001u

#define SRR1_DMA_BIT                0x00200000u
#define SRR1_L2DP_BIT               0x00100000u

#define L2CR_L2E                    0x80000000u
#define L2CR_L2PE                   0x40000000u
#define L2CR_L2SIZ_256K             0x10000000u
#define L2CR_L2SIZ_512K             0x20000000u
#define L2CR_L2SIZ_1M               0x30000000u
#define L2CR_L2CLK_1_0              0x02000000u
#define L2CR_L2CLK_1_5              0x04000000u
#define L2CR_L2CLK_2_0              0x08000000u
#define L2CR_L2CLK_2_5              0x0A000000u
#define L2CR_L2CLK_3_0              0x0C000000u
#define L2CR_RAM_FLOW_THRU_BURST    0x00000000u
#define L2CR_RAM_PIPELINE_BURST     0x01000000u
#define L2CR_RAM_PIPELINE_LATE      0x01800000u
#define L2CR_L2DO                   0x00400000u
#define L2CR_L2I                    0x00200000u
#define L2CR_L2CTL                  0x00100000u
#define L2CR_L2WT                   0x00080000u
#define L2CR_L2TS                   0x00040000u
#define L2CR_L2OH_0_5               0x00000000u
#define L2CR_L2OH_1_0               0x00010000u
#define L2CR_L2SL                   0x00008000u
#define L2CR_L2DF                   0x00004000u
#define L2CR_L2BYP                  0x00002000u
#define L2CR_L2CS                   0x00000200u
#define L2CR_L2DRO                  0x00000100u
#define L2CR_L2CTR_MASK             0x000000FEu
#define L2CR_L2IP                   0x00000001u

// ---------------------------------------------------------------------------
// MMCR0 / MMCR1 bits
// ---------------------------------------------------------------------------
#define MMCR0_DIS                   0x80000000u
#define MMCR0_DP                    0x40000000u
#define MMCR0_DU                    0x20000000u
#define MMCR0_DMS                   0x10000000u
#define MMCR0_DMR                   0x08000000u
#define MMCR0_ENINT                 0x04000000u
#define MMCR0_DISCOUNT              0x02000000u
#define MMCR0_RTCSELECT_MASK        0x01800000u
#define MMCR0_RTCSELECT_63          0x00000000u
#define MMCR0_RTCSELECT_55          0x00800000u
#define MMCR0_RTCSELECT_51          0x01000000u
#define MMCR0_RTCSELECT_47          0x01800000u
#define MMCR0_INTONBITTRANS         0x00400000u
#define MMCR0_THRESHOLD_MASK        0x003F0000u
#define MMCR0_THRESHOLD(n)          ((n) << 16)
#define MMCR0_PMC1INTCONTROL        0x00008000u
#define MMCR0_PMC2INTCONTROL        0x00004000u
#define MMCR0_PMCTRIGGER            0x00002000u
#define MMCR0_PMC1SELECT_MASK       0x00001FC0u
#define MMCR0_PMC2SELECT_MASK       0x0000003Fu

#define MMCR1_PMC3SELECT_MASK       0xF8000000u
#define MMCR1_PMC4SELECT_MASK       0x07C00000u

#define PMC1_OV                     0x80000000u
#define PMC1_COUNTER                0x7FFFFFFFu
#define PMC2_OV                     0x80000000u
#define PMC2_COUNTER                0x7FFFFFFFu
#define PMC3_OV                     0x80000000u
#define PMC3_COUNTER                0x7FFFFFFFu
#define PMC4_OV                     0x80000000u
#define PMC4_COUNTER                0x7FFFFFFFu

// PMC1 events
#define MMCR0_PMC1_HOLD             0x00000000u
#define MMCR0_PMC1_CYCLE            0x00000040u
#define MMCR0_PMC1_INSTRUCTION      0x00000080u
#define MMCR0_PMC1_TRANSITION       0x000000C0u
#define MMCR0_PMC1_DISPATCHED       0x00000100u
#define MMCR0_PMC1_EIEIO            0x00000140u
#define MMCR0_PMC1_ITLB_CYCLE       0x00000180u
#define MMCR0_PMC1_L2_HIT           0x000001C0u
#define MMCR0_PMC1_EA               0x00000200u
#define MMCR0_PMC1_IABR             0x00000240u
#define MMCR0_PMC1_L1_MISS          0x00000280u
#define MMCR0_PMC1_Bx_UNRESOLVED    0x000002C0u
#define MMCR0_PMC1_Bx_STALL_CYCLE   0x00000300u
#define MMCR0_PMC1_IC_FETCH_MISS    0x00000340u

// PMC2 events
#define MMCR0_PMC2_HOLD             0x00000000u
#define MMCR0_PMC2_CYCLE            0x00000001u
#define MMCR0_PMC2_INSTRUCTION      0x00000002u
#define MMCR0_PMC2_TRANSITION       0x00000003u
#define MMCR0_PMC2_DISPATCHED       0x00000004u
#define MMCR0_PMC2_IC_MISS          0x00000005u
#define MMCR0_PMC2_ITLB_MISS        0x00000006u
#define MMCR0_PMC2_L2_I_MISS        0x00000007u
#define MMCR0_PMC2_Bx_FALL_TROUGH   0x00000008u
#define MMCR0_PMC2_PR_SWITCH        0x00000009u
#define MMCR0_PMC2_RESERVED_LOAD    0x0000000Au
#define MMCR0_PMC2_LOAD_STORE       0x0000000Bu
#define MMCR0_PMC2_SNOOP            0x0000000Cu
#define MMCR0_PMC2_L1_CASTOUT       0x0000000Du
#define MMCR0_PMC2_SYSTEM           0x0000000Eu
#define MMCR0_PMC2_IC_FETCH_MISS    0x0000000Fu
#define MMCR0_PMC2_Bx_OUT_OF_ORDER  0x00000010u

// PMC3 events
#define MMCR1_PMC3_HOLD             0x00000000u
#define MMCR1_PMC3_CYCLE            0x08000000u
#define MMCR1_PMC3_INSTRUCTION      0x10000000u
#define MMCR1_PMC3_TRANSITION       0x18000000u
#define MMCR1_PMC3_DISPATCHED       0x20000000u
#define MMCR1_PMC3_DC_MISS          0x28000000u
#define MMCR1_PMC3_DTLB_MISS        0x30000000u
#define MMCR1_PMC3_L2_D_MISS        0x38000000u
#define MMCR1_PMC3_Bx_TAKEN         0x40000000u
#define MMCR1_PMC3_PM_SWITCH        0x48000000u
#define MMCR1_PMC3_COND_STORE       0x50000000u
#define MMCR1_PMC3_FPU              0x58000000u
#define MMCR1_PMC3_L2_SNOOP_CASTOUT 0x60000000u
#define MMCR1_PMC3_L2_HIT           0x68000000u
#define MMCR1_PMC3_L1_MISS_CYCLE    0x78000000u
#define MMCR1_PMC3_Bx_SECOND        0x80000000u
#define MMCR1_PMC3_BPU_LR_CR        0x88000000u

// PMC4 events
#define MMCR1_PMC4_HOLD             0x00000000u
#define MMCR1_PMC4_CYCLE            0x00400000u
#define MMCR1_PMC4_INSTRUCTION      0x00800000u
#define MMCR1_PMC4_TRANSITION       0x00C00000u
#define MMCR1_PMC4_DISPATCHED       0x01000000u
#define MMCR1_PMC4_L2_CASTOUT       0x01400000u
#define MMCR1_PMC4_DTLB_CYCLE       0x01800000u
#define MMCR1_PMC4_Bx_MISSED        0x02000000u
#define MMCR1_PMC4_COND_STORE_INT   0x02800000u
#define MMCR1_PMC4_SYNC             0x02C00000u
#define MMCR1_PMC4_SNOOP_RETRY      0x03000000u
#define MMCR1_PMC4_INTEGER          0x03400000u
#define MMCR1_PMC4_BPU_THIRD        0x03800000u
#define MMCR1_PMC4_DC_MISS          0x07C00000u

// ---------------------------------------------------------------------------
// FPSCR bits
// ---------------------------------------------------------------------------
#ifndef FPSCR_FX
#define FPSCR_FX            0x80000000u
#define FPSCR_FEX           0x40000000u
#define FPSCR_VX            0x20000000u
#define FPSCR_OX            0x10000000u
#define FPSCR_UX            0x08000000u
#define FPSCR_ZX            0x04000000u
#define FPSCR_XX            0x02000000u
#define FPSCR_VXSNAN        0x01000000u
#define FPSCR_VXISI         0x00800000u
#define FPSCR_VXIDI         0x00400000u
#define FPSCR_VXZDZ         0x00200000u
#define FPSCR_VXIMZ         0x00100000u
#define FPSCR_VXVC          0x00080000u
#define FPSCR_FR            0x00040000u
#define FPSCR_FI            0x00020000u
#define FPSCR_VXSOFT        0x00000400u
#define FPSCR_VXSQRT        0x00000200u
#define FPSCR_VXCVI         0x00000100u
#define FPSCR_VE            0x00000080u
#define FPSCR_OE            0x00000040u
#define FPSCR_UE            0x00000020u
#define FPSCR_ZE            0x00000010u
#define FPSCR_XE            0x00000008u
#define FPSCR_NI            0x00000004u
#endif

#ifndef FPSCR_FX_BIT
#define FPSCR_FX_BIT        0
#define FPSCR_FEX_BIT       1
#define FPSCR_VX_BIT        2
#define FPSCR_OX_BIT        3
#define FPSCR_UX_BIT        4
#define FPSCR_ZX_BIT        5
#define FPSCR_XX_BIT        6
#define FPSCR_VXSNAN_BIT    7
#define FPSCR_VXISI_BIT     8
#define FPSCR_VXIDI_BIT     9
#define FPSCR_VXZDZ_BIT     10
#define FPSCR_VXIMZ_BIT     11
#define FPSCR_VXVC_BIT      12
#define FPSCR_FR_BIT        13
#define FPSCR_FI_BIT        14
#define FPSCR_VXSOFT_BIT    21
#define FPSCR_VXSQRT_BIT    22
#define FPSCR_VXCVI_BIT     23
#define FPSCR_VE_BIT        24
#define FPSCR_OE_BIT        25
#define FPSCR_UE_BIT        26
#define FPSCR_ZE_BIT        27
#define FPSCR_XE_BIT        28
#define FPSCR_NI_BIT        29
#endif

union FpscrUnion {
    f64 f;
    struct {
        u32 fpscr_pad;
        u32 fpscr;
    } u;
};

// ---------------------------------------------------------------------------
// PPCArch function stubs — all hardware ops are no-ops on PC
// mfspr/mtspr are replaced with plain C returning 0 or doing nothing.
// ---------------------------------------------------------------------------
static inline u32  PPCMfmsr(void)            { return 0; }
static inline void PPCMtmsr(u32 v)           { (void)v; }
static inline u32  PPCOrMsr(u32 v)           { (void)v; return 0; }
static inline u32  PPCAndMsr(u32 v)          { (void)v; return 0; }
static inline u32  PPCAndCMsr(u32 v)         { (void)v; return 0; }
static inline u32  PPCMfhid0(void)           { return 0; }
static inline void PPCMthid0(u32 v)          { (void)v; }
static inline u32  PPCMfhid1(void)           { return 0; }
static inline u32  PPCMfhid2(void)           { return 0; }
static inline void PPCMthid2(u32 v)          { (void)v; }
static inline u32  PPCMfl2cr(void)           { return 0; }
static inline void PPCMtl2cr(u32 v)          { (void)v; }
static inline void PPCMtdec(u32 v)           { (void)v; }
static inline u32  PPCMfdec(void)            { return 0; }
static inline void PPCSync(void)             {}
static inline void PPCHalt(void)             { abort(); }
static inline u32  PPCMffpscr(void)          { return 0; }
static inline void PPCMtfpscr(u32 v)         { (void)v; }
static inline u32  PPCMfwpar(void)           { return 0; }
static inline void PPCMtwpar(u32 v)          { (void)v; }
static inline void PPCEnableSpeculation(void) {}
static inline void PPCDisableSpeculation(void) {}
static inline void PPCSetFpIEEEMode(void)    {}
static inline void PPCSetFpNonIEEEMode(void) {}
static inline u32  PPCMfpmc1(void)           { return 0; }
static inline u32  PPCMfpmc2(void)           { return 0; }
static inline u32  PPCMfpmc3(void)           { return 0; }
static inline u32  PPCMfpmc4(void)           { return 0; }
static inline void PPCMtpmc1(u32 v)          { (void)v; }
static inline void PPCMtpmc2(u32 v)          { (void)v; }
static inline void PPCMtpmc3(u32 v)          { (void)v; }
static inline void PPCMtpmc4(u32 v)          { (void)v; }
static inline u32  PPCMfmmcr0(void)          { return 0; }
static inline void PPCMtmmcr0(u32 v)         { (void)v; }
static inline u32  PPCMfmmcr1(void)          { return 0; }
static inline void PPCMtmmcr1(u32 v)         { (void)v; }
static inline u32  PPCMfsia(void)            { return 0; }
static inline void PPCMtsia(u32 v)           { (void)v; }
static inline u32  PPCMfdmaL(void)           { return 0; }
static inline u32  PPCMfdmaU(void)           { return 0; }
static inline void PPCMtdmaU(u32 v)          { (void)v; }
static inline void PPCMtdmaL(u32 v)          { (void)v; }
static inline u32  PPCMfpvr(void)            { return 0; }
static inline void PPCEieio(void)            {}

// ---------------------------------------------------------------------------
// PPCPm (performance monitor) stubs
// ---------------------------------------------------------------------------
static inline void PMBegin(void)         {}
static inline void PMEnd(void)           {}
static inline void PMCycles(void)        {}
static inline void PML1FetchMisses(void) {}
static inline void PML1MissCycles(void)  {}
static inline void PMInstructions(void)  {}

#ifdef __cplusplus
}
#endif
