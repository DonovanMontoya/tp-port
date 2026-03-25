#pragma once
/**
 * PC stub for dolphin/axart.h
 * All articulator struct types and the enum are kept identical to the
 * original.  The processing functions are no-ops.
 *
 * Note: the original uses AXVPB; the port ax.h renames that type to AXPB.
 * We forward-declare AXPB here and typedef AXVPB as an alias so that any
 * source file that refers to AXVPB by name still compiles.
 */

#include "port/types.h"
#include "dolphin/ax.h"

#ifdef __cplusplus
extern "C" {
#endif

// Allow code written against the original SDK name to compile
#ifndef AXVPB
typedef AXPB AXVPB;
#endif

// ---------------------------------------------------------------------------
// Articulator type enum
// ---------------------------------------------------------------------------
enum __axart_type {
    AXART_TYPE_NONE,
    AXART_TYPE_3D,
    AXART_TYPE_PANNING,
    AXART_TYPE_ITD,
    AXART_TYPE_SRC,
    AXART_TYPE_PITCH,
    AXART_TYPE_PITCH_ENV,
    AXART_TYPE_PITCH_MOD,
    AXART_TYPE_VOLUME,
    AXART_TYPE_AUX_A_VOLUME,
    AXART_TYPE_AUX_B_VOLUME,
    AXART_TYPE_VOLUME_ENV,
    AXART_TYPE_AUX_A_VOLUME_ENV,
    AXART_TYPE_AUX_B_VOLUME_ENV,
    AXART_TYPE_VOLUME_MOD,
    AXART_TYPE_AUX_A_VOLUME_MOD,
    AXART_TYPE_AUX_B_VOLUME_MOD,
    AXART_TYPE_LPF,

    AXART_TYPE_NUM
};

// ---------------------------------------------------------------------------
// Base articulator link node
// ---------------------------------------------------------------------------
typedef struct {
    void* next;
    u32   type;
} AXART_ART;

// ---------------------------------------------------------------------------
// LFO state
// ---------------------------------------------------------------------------
typedef struct {
    f32* lfo;
    u32  length;
    f32  delta;
    u32  sampleIndex;
    f32  counter;
    f32  sample1;
    f32  sample;
    f32  output;
} AXART_LFO;

// ---------------------------------------------------------------------------
// Articulator subtypes
// ---------------------------------------------------------------------------
typedef struct {
    AXART_ART art;
    f32 hAngle;
    f32 vAngle;
    f32 dist;
    f32 closingSpeed;
    u32 update;
    u8  pan;
    u8  span;
    u8  src;
    u16 itdL;
    u16 itdR;
    f32 pitch;
    s32 attenuation;
} AXART_3D;

typedef struct {
    AXART_ART art;
    u8 pan;
    u8 span;
} AXART_PANNING;

typedef struct {
    AXART_ART art;
    u16 itdL;
    u16 itdR;
} AXART_ITD;

typedef struct {
    AXART_ART art;
    u8 src;
} AXART_SRC;

typedef struct {
    AXART_ART art;
    s32 cents;
} AXART_PITCH;

typedef struct {
    AXART_ART art;
    s32 delta;
    s32 target;
    s32 cents;
} AXART_PITCH_ENV;

typedef struct {
    AXART_ART art;
    AXART_LFO lfo;
    s32 cents;
} AXART_PITCH_MOD;

typedef struct {
    AXART_ART art;
    s32 attenuation;
} AXART_VOLUME;

typedef struct {
    AXART_ART art;
    s32 attenuation;
} AXART_AUXA_VOLUME;

typedef struct {
    AXART_ART art;
    s32 attenuation;
} AXART_AUXB_VOLUME;

typedef struct {
    AXART_ART art;
    s32 delta;
    s32 target;
    s32 attenuation;
} AXART_VOLUME_ENV;

typedef struct {
    AXART_ART art;
    s32 delta;
    s32 target;
    s32 attenuation;
} AXART_AUXA_VOLUME_ENV;

typedef struct {
    AXART_ART art;
    s32 delta;
    s32 target;
    s32 attenuation;
} AXART_AUXB_VOLUME_ENV;

typedef struct {
    AXART_ART art;
    AXART_LFO lfo;
    s32 attenuation;
} AXART_VOLUME_MOD;

typedef struct {
    AXART_ART art;
    AXART_LFO lfo;
    s32 attenuation;
} AXART_AUXA_VOLUME_MOD;

typedef struct {
    AXART_ART art;
    AXART_LFO lfo;
    s32 attenuation;
} AXART_AUXB_VOLUME_MOD;

typedef struct {
    AXART_ART art;
    u32 initLPF;
    u32 frequency;
    u32 update;
} AXART_LPF;

// ---------------------------------------------------------------------------
// Sound node
// ---------------------------------------------------------------------------
typedef struct {
    void*      next;
    void*      prev;
    AXPB*      axvpb;
    f32        sampleRate;
    AXART_ART* articulators;
} AXART_SOUND;

// ---------------------------------------------------------------------------
// Sine LUT (64-entry table; defined in src/dolphin/axart.cpp)
// ---------------------------------------------------------------------------
#define AXART_SINE_CNT 64
extern f32 AXARTSine[AXART_SINE_CNT];

// ---------------------------------------------------------------------------
// Function stubs — all are no-ops on PC
// ---------------------------------------------------------------------------

// axart
static inline void AXARTInit(void)                                          {}
static inline void AXARTQuit(void)                                          {}
static inline void AXARTServiceSounds(void)                                 {}
static inline void AXARTAddSound(AXART_SOUND* s)                            { (void)s; }
static inline void AXARTRemoveSound(AXART_SOUND* s)                         { (void)s; }
static inline void AXARTInitLfo(AXART_LFO* lfo, f32* samples, u32 length, f32 delta)
    { (void)lfo; (void)samples; (void)length; (void)delta; }
static inline void AXARTInitArt3D(AXART_3D* a)                              { (void)a; }
static inline void AXARTInitArtPanning(AXART_PANNING* a)                    { (void)a; }
static inline void AXARTInitArtItd(AXART_ITD* a)                            { (void)a; }
static inline void AXARTInitArtSrctype(AXART_SRC* a)                        { (void)a; }
static inline void AXARTInitArtPitch(AXART_PITCH* a)                        { (void)a; }
static inline void AXARTInitArtPitchEnv(AXART_PITCH_ENV* a)                 { (void)a; }
static inline void AXARTInitArtPitchMod(AXART_PITCH_MOD* a)                 { (void)a; }
static inline void AXARTInitArtVolume(AXART_VOLUME* a)                      { (void)a; }
static inline void AXARTInitArtAuxAVolume(AXART_AUXA_VOLUME* a)             { (void)a; }
static inline void AXARTInitArtAuxBVolume(AXART_AUXB_VOLUME* a)             { (void)a; }
static inline void AXARTInitArtVolumeEnv(AXART_VOLUME_ENV* a)               { (void)a; }
static inline void AXARTInitArtAuxAVolumeEnv(AXART_AUXA_VOLUME_ENV* a)      { (void)a; }
static inline void AXARTInitArtAuxBVolumeEnv(AXART_AUXB_VOLUME_ENV* a)      { (void)a; }
static inline void AXARTInitArtVolumeMod(AXART_VOLUME_MOD* a)               { (void)a; }
static inline void AXARTInitArtAuxAVolumeMod(AXART_AUXA_VOLUME_MOD* a)      { (void)a; }
static inline void AXARTInitArtAuxBVolumeMod(AXART_AUXB_VOLUME_MOD* a)      { (void)a; }
static inline void AXARTInitArtLpf(AXART_LPF* a)                            { (void)a; }

// axart3d
static inline void AXARTSet3DDistanceScale(f32 scale)                       { (void)scale; }
static inline void AXARTSet3DDopplerScale(f32 scale)                        { (void)scale; }
static inline void AXART3DSound(AXART_3D* a)                                { (void)a; }

// axartcents
static inline f32  AXARTCents(s32 cents)                                    { (void)cents; return 1.0f; }

// axartenv
static inline void AXARTPitchEnv(AXART_PITCH_ENV* a)                        { (void)a; }
static inline void AXARTVolumeEnv(AXART_VOLUME_ENV* a)                      { (void)a; }

// axartlfo
static inline void AXARTLfo(AXART_LFO* lfo)                                 { (void)lfo; }

// axartsound
static inline void AXARTServiceSound(AXART_SOUND* s)                        { (void)s; }
static inline void AXARTAddArticulator(AXART_SOUND* s, AXART_ART* a)        { (void)s; (void)a; }

// axartlpf
static inline void AXARTLpf(AXART_LPF* a, AXPB* pb)                        { (void)a; (void)pb; }

#ifdef __cplusplus
}
#endif
