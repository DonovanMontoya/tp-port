#pragma once
/**
 * PC stub for dolphin/ai.h
 * The AI hardware DMA and streaming API is stubbed out.
 * Actual audio output goes through OpenAL via the AX layer.
 * All query functions return safe default values.
 */

#include "port/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*AISCallback)(u32 count);
typedef void (*AIDCallback)(void);

#define AI_STREAM_START  1
#define AI_STREAM_STOP   0

#define AI_SAMPLERATE_32KHZ  0
#define AI_SAMPLERATE_48KHZ  1

// DMA callback registration — returns previous callback
static inline AIDCallback AIRegisterDMACallback(AIDCallback callback)
    { (void)callback; return NULL; }

// DMA control
static inline void AIInitDMA(u32 start_addr, u32 length)
    { (void)start_addr; (void)length; }
static inline BOOL AIGetDMAEnableFlag(void)    { return FALSE; }
static inline void AIStartDMA(void)            {}
static inline void AIStopDMA(void)             {}
static inline u32  AIGetDMABytesLeft(void)     { return 0; }
static inline u32  AIGetDMAStartAddr(void)     { return 0; }
static inline u32  AIGetDMALength(void)        { return 0; }
static inline BOOL AICheckInit(void)           { return TRUE; }

// Streaming sample-counter callbacks
static inline AISCallback AIRegisterStreamCallback(AISCallback callback)
    { (void)callback; return NULL; }
static inline u32  AIGetStreamSampleCount(void)           { return 0; }
static inline void AIResetStreamSampleCount(void)         {}
static inline void AISetStreamTrigger(u32 trigger)        { (void)trigger; }
static inline u32  AIGetStreamTrigger(void)               { return 0; }

// Stream play-state
static inline void AISetStreamPlayState(u32 state)        { (void)state; }
static inline u32  AIGetStreamPlayState(void)             { return AI_STREAM_STOP; }

// Sample-rate selectors
static inline void AISetDSPSampleRate(u32 rate)           { (void)rate; }
static inline u32  AIGetDSPSampleRate(void)               { return AI_SAMPLERATE_32KHZ; }
static inline void AISetStreamSampleRate(u32 rate)        { (void)rate; }
static inline u32  AIGetStreamSampleRate(void)            { return AI_SAMPLERATE_32KHZ; }

// Volume
static inline void AISetStreamVolLeft(u8 vol)             { (void)vol; }
static inline u8   AIGetStreamVolLeft(void)               { return 0xFF; }
static inline void AISetStreamVolRight(u8 vol)            { (void)vol; }
static inline u8   AIGetStreamVolRight(void)              { return 0xFF; }

// Init / reset
static inline void AIInit(u8* stack)                      { (void)stack; }
static inline void AIReset(void)                          {}

#ifdef __cplusplus
}
#endif
