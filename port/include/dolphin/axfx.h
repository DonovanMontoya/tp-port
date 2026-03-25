#pragma once
/**
 * PC stub for dolphin/axfx.h
 * All reverb/delay/chorus effect structs are kept in full so that callers
 * that embed them (e.g. game audio managers) compile without modification.
 * The processing callbacks are no-ops — actual DSP effects would need a
 * software implementation in src/dolphin/axfx.cpp if desired.
 */

#include "port/types.h"

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
// Buffer-update descriptors (passed to all effect callbacks)
// ---------------------------------------------------------------------------
typedef struct AXFX_BUFFERUPDATE {
    /* 0x00 */ s32* left;
    /* 0x04 */ s32* right;
    /* 0x08 */ s32* surround;
} AXFX_BUFFERUPDATE;

typedef struct AXFX_BUFFERUPDATE_DPL2 {
    /* 0x00 */ s32* L;
    /* 0x04 */ s32* R;
    /* 0x08 */ s32* Ls;
    /* 0x0C */ s32* Rs;
} AXFX_BUFFERUPDATE_DPL2;

// ---------------------------------------------------------------------------
// Standard reverb
// ---------------------------------------------------------------------------
typedef struct AXFX_REVSTD_DELAYLINE {
    /* 0x00 */ s32   inPoint;
    /* 0x04 */ s32   outPoint;
    /* 0x08 */ s32   length;
    /* 0x0C */ f32*  inputs;
    /* 0x10 */ f32   lastOutput;
} AXFX_REVSTD_DELAYLINE;

typedef struct AXFX_REVSTD_WORK {
    /* 0x000 */ AXFX_REVSTD_DELAYLINE AP[6];
    /* 0x078 */ AXFX_REVSTD_DELAYLINE C[6];
    /* 0x0F0 */ f32 allPassCoeff;
    /* 0x0F4 */ f32 combCoef[6];
    /* 0x10C */ f32 lpLastout[3];
    /* 0x118 */ f32 level;
    /* 0x11C */ f32 damping;
    /* 0x120 */ s32 preDelayTime;
    /* 0x124 */ f32* preDelayLine[3];
    /* 0x130 */ f32* preDelayPtr[3];
} AXFX_REVSTD_WORK;

typedef struct AXFX_REVERBSTD {
    /* 0x000 */ AXFX_REVSTD_WORK rv;
    /* 0x13C */ u8  tempDisableFX;
    /* 0x140 */ f32 coloration;
    /* 0x144 */ f32 mix;
    /* 0x148 */ f32 time;
    /* 0x14C */ f32 damping;
    /* 0x150 */ f32 preDelay;
} AXFX_REVERBSTD;

// ---------------------------------------------------------------------------
// High-quality reverb (stereo)
// ---------------------------------------------------------------------------
typedef struct AXFX_REVHI_DELAYLINE {
    /* 0x00 */ s32   inPoint;
    /* 0x04 */ s32   outPoint;
    /* 0x08 */ s32   length;
    /* 0x0C */ f32*  inputs;
    /* 0x10 */ f32   lastOutput;
} AXFX_REVHI_DELAYLINE;

typedef struct AXFX_REVHI_WORK {
    /* 0x000 */ AXFX_REVHI_DELAYLINE AP[9];
    /* 0x0B4 */ AXFX_REVHI_DELAYLINE C[9];
    /* 0x168 */ f32 allPassCoeff;
    /* 0x16C */ f32 combCoef[9];
    /* 0x190 */ f32 lpLastout[3];
    /* 0x19C */ f32 level;
    /* 0x1A0 */ f32 damping;
    /* 0x1A4 */ s32 preDelayTime;
    /* 0x1A8 */ f32 crosstalk;
    /* 0x1AC */ f32* preDelayLine[3];
    /* 0x1B8 */ f32* preDelayPtr[3];
} AXFX_REVHI_WORK;

typedef struct AXFX_REVERBHI {
    /* 0x000 */ AXFX_REVHI_WORK rv;
    /* 0x1C4 */ u8  tempDisableFX;
    /* 0x1C8 */ f32 coloration;
    /* 0x1CC */ f32 mix;
    /* 0x1D0 */ f32 time;
    /* 0x1D4 */ f32 damping;
    /* 0x1D8 */ f32 preDelay;
    /* 0x1DC */ f32 crosstalk;
} AXFX_REVERBHI;

// ---------------------------------------------------------------------------
// High-quality reverb — Dolby Pro Logic II (4-channel)
// ---------------------------------------------------------------------------
typedef struct AXFX_REVHI_WORK_DPL2 {
    /* 0x000 */ AXFX_REVHI_DELAYLINE AP[12];
    /* 0x0F0 */ AXFX_REVHI_DELAYLINE C[12];
    /* 0x1E0 */ f32 allPassCoeff;
    /* 0x1E4 */ f32 combCoef[12];
    /* 0x214 */ f32 lpLastout[4];
    /* 0x224 */ f32 level;
    /* 0x228 */ f32 damping;
    /* 0x22C */ s32 preDelayTime;
    /* 0x230 */ f32 crosstalk;
    /* 0x234 */ f32* preDelayLine[4];
    /* 0x244 */ f32* preDelayPtr[4];
} AXFX_REVHI_WORK_DPL2;

typedef struct AXFX_REVERBHI_DPL2 {
    /* 0x000 */ AXFX_REVHI_WORK_DPL2 rv;
    /* 0x254 */ u8  tempDisableFX;
    /* 0x258 */ f32 coloration;
    /* 0x25C */ f32 mix;
    /* 0x260 */ f32 time;
    /* 0x264 */ f32 damping;
    /* 0x268 */ f32 preDelay;
} AXFX_REVERBHI_DPL2;

// ---------------------------------------------------------------------------
// Delay effect
// ---------------------------------------------------------------------------
typedef struct AXFX_DELAY {
    /* 0x00 */ u32  currentSize[3];
    /* 0x0C */ u32  currentPos[3];
    /* 0x18 */ u32  currentFeedback[3];
    /* 0x24 */ u32  currentOutput[3];
    /* 0x30 */ s32* left;
    /* 0x34 */ s32* right;
    /* 0x38 */ s32* sur;
    /* 0x3C */ u32  delay[3];
    /* 0x48 */ u32  feedback[3];
    /* 0x54 */ u32  output[3];
} AXFX_DELAY;

// ---------------------------------------------------------------------------
// Chorus effect
// ---------------------------------------------------------------------------
typedef struct AXFX_CHORUS_SRCINFO {
    /* 0x00 */ s32* dest;
    /* 0x04 */ s32* smpBase;
    /* 0x08 */ s32* old;
    /* 0x0C */ u32  posLo;
    /* 0x10 */ u32  posHi;
    /* 0x14 */ u32  pitchLo;
    /* 0x18 */ u32  pitchHi;
    /* 0x1C */ u32  trigger;
    /* 0x20 */ u32  target;
} AXFX_CHORUS_SRCINFO;

typedef struct AXFX_CHORUS_WORK {
    /* 0x00 */ s32* lastLeft[3];
    /* 0x0C */ s32* lastRight[3];
    /* 0x18 */ s32* lastSur[3];
    /* 0x24 */ u8   currentLast;
    /* 0x28 */ s32  oldLeft[4];
    /* 0x38 */ s32  oldRight[4];
    /* 0x48 */ s32  oldSur[4];
    /* 0x58 */ u32  currentPosLo;
    /* 0x5C */ u32  currentPosHi;
    /* 0x60 */ s32  pitchOffset;
    /* 0x64 */ u32  pitchOffsetPeriodCount;
    /* 0x68 */ u32  pitchOffsetPeriod;
    /* 0x6C */ AXFX_CHORUS_SRCINFO src;
} AXFX_CHORUS_WORK;

typedef struct AXFX_CHORUS {
    /* 0x00 */ AXFX_CHORUS_WORK work;
    /* 0x90 */ u32 baseDelay;
    /* 0x94 */ u32 variation;
    /* 0x98 */ u32 period;
} AXFX_CHORUS;

// ---------------------------------------------------------------------------
// Function stubs — init/shutdown return 1 (success), callbacks are no-ops
// ---------------------------------------------------------------------------

// chorus
static inline int  AXFXChorusInit(AXFX_CHORUS* c)          { (void)c; return 1; }
static inline int  AXFXChorusShutdown(AXFX_CHORUS* c)      { (void)c; return 1; }
static inline int  AXFXChorusSettings(AXFX_CHORUS* c)      { (void)c; return 1; }
static inline void AXFXChorusCallback(AXFX_BUFFERUPDATE* b, AXFX_CHORUS* c) { (void)b; (void)c; }

// delay
static inline void AXFXDelayCallback(AXFX_BUFFERUPDATE* b, AXFX_DELAY* d) { (void)b; (void)d; }
static inline int  AXFXDelaySettings(AXFX_DELAY* d)        { (void)d; return 1; }
static inline int  AXFXDelayInit(AXFX_DELAY* d)            { (void)d; return 1; }
static inline int  AXFXDelayShutdown(AXFX_DELAY* d)        { (void)d; return 1; }

// reverb_hi
static inline void DoCrossTalk(s32* l, s32* r, f32 cross, f32 invcross)
    { (void)l; (void)r; (void)cross; (void)invcross; }
static inline int  AXFXReverbHiInit(AXFX_REVERBHI* rev)         { (void)rev; return 1; }
static inline int  AXFXReverbHiShutdown(AXFX_REVERBHI* rev)     { (void)rev; return 1; }
static inline int  AXFXReverbHiSettings(AXFX_REVERBHI* rev)     { (void)rev; return 1; }
static inline void AXFXReverbHiCallback(AXFX_BUFFERUPDATE* b, AXFX_REVERBHI* rev)
    { (void)b; (void)rev; }

// reverb_hi_4ch (DPL2)
static inline int  AXFXReverbHiInitDpl2(AXFX_REVERBHI_DPL2* rev)     { (void)rev; return 1; }
static inline int  AXFXReverbHiShutdownDpl2(AXFX_REVERBHI_DPL2* rev) { (void)rev; return 1; }
static inline int  AXFXReverbHiSettingsDpl2(AXFX_REVERBHI_DPL2* rev) { (void)rev; return 1; }
static inline void AXFXReverbHiCallbackDpl2(AXFX_BUFFERUPDATE_DPL2* b, AXFX_REVERBHI_DPL2* rev)
    { (void)b; (void)rev; }

// reverb_std
static inline int  AXFXReverbStdInit(AXFX_REVERBSTD* rev)     { (void)rev; return 1; }
static inline int  AXFXReverbStdShutdown(AXFX_REVERBSTD* rev) { (void)rev; return 1; }
static inline int  AXFXReverbStdSettings(AXFX_REVERBSTD* rev) { (void)rev; return 1; }
static inline void AXFXReverbStdCallback(AXFX_BUFFERUPDATE* b, AXFX_REVERBSTD* rev)
    { (void)b; (void)rev; }

#ifdef __cplusplus
}
#endif
