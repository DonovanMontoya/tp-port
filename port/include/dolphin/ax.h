#pragma once
/**
 * PC stub for dolphin/ax.h
 * Maps the GameCube DSP/AX audio API to OpenAL.
 * The AX system processes audio voices at 32 kHz in 5 ms frames.
 * Implementation: src/dolphin/ax.cpp
 */
#include "port/types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Voice counts (same as GC hardware limit)
#define AX_MAX_VOICES 64

// AX voice state
#define AX_PB_STATE_RUN  0x8000
#define AX_PB_STATE_STOP 0x0000

// Sample format
#define AXPB_FORMAT_ADPCM     0
#define AXPB_FORMAT_PCM16     0x0A
#define AXPB_FORMAT_PCM8      0x19

// Loop type
#define AXPB_LOOPFLAG_NOLOOP 0
#define AXPB_LOOPFLAG_LOOP   1

typedef struct _AXPBMIX {
    u16 vL, vDeltaL, vR, vDeltaR;
    u16 vAuxAL, vDeltaAuxAL, vAuxAR, vDeltaAuxAR;
    u16 vAuxBL, vDeltaAuxBL, vAuxBR, vDeltaAuxBR;
    u16 vAuxBS, vDeltaAuxBS;
    u16 vS, vDeltaS;
    u16 vAuxAS, vDeltaAuxAS;
} AXPBMIX;

typedef struct _AXPBVE {
    u16 currentVolume;
    s16 currentDelta;
} AXPBVE;

typedef struct _AXPBADDR {
    u16 loopFlag;
    u16 format;
    u16 loopAddrHi, loopAddrLo;
    u16 endAddrHi,  endAddrLo;
    u16 curAddrHi,  curAddrLo;
} AXPBADDR;

typedef struct _AXPBADPCM {
    s16 coef[8][2];
    u16 gain;
    u16 pred_scale;
    s16 yn1, yn2;
} AXPBADPCM;

typedef struct _AXPBSRC {
    u16 ratioHi, ratioLo;
    u16 currentAddressFrac;
    s16 last_samples[4];
} AXPBSRC;

typedef struct AXPB {
    u16       state;
    u16       mixing;
    u16       type;
    u16       axOutBufferIndex;
    u16       priority;
    u16       syncFlags;
    AXPBMIX   mix;
    AXPBVE    ve;
    AXPBADDR  addr;
    AXPBADPCM adpcm;
    AXPBSRC   src;
    // PC-only: OpenAL source handle
    u32       _alSource;
} AXPB;

typedef void (*AXAudioFrameCallback)(void);
typedef void (*AXUserCallback)(void);

// AX lifecycle
void AXInit(void);
void AXQuit(void);

// Voice management
AXPB* AXAcquireVoice(u32 priority, AXUserCallback cb, u32 cbData);
void  AXFreeVoice(AXPB* pb);
void  AXSetVoiceState(AXPB* pb, u16 state);
void  AXSetVoiceAddr(AXPB* pb, const AXPBADDR* addr);
void  AXSetVoiceAdpcm(AXPB* pb, const AXPBADPCM* adpcm);
void  AXSetVoiceSrc(AXPB* pb, const AXPBSRC* src);
void  AXSetVoiceSrcType(AXPB* pb, u32 type);
void  AXSetVoiceSrcRatio(AXPB* pb, f32 ratio);
void  AXSetVoiceMix(AXPB* pb, const AXPBMIX* mix);
void  AXSetVoiceVe(AXPB* pb, const AXPBVE* ve);
void  AXSetVoiceAdpcmData(AXPB* pb, u16 predScale, s16 yn1, s16 yn2);
void  AXSetVoiceLoop(AXPB* pb, u16 loopFlag);
void  AXSetVoiceLoopAddr(AXPB* pb, u32 addr);
void  AXSetVoiceEndAddr(AXPB* pb, u32 addr);
void  AXSetVoiceCurrentAddr(AXPB* pb, u32 addr);
u32   AXGetVoiceCurrentAddr(AXPB* pb);
BOOL  AXIsVoiceRunning(AXPB* pb);
u32   AXGetInputSamplesPerFrame(void);
u32   AXGetInputSamplesPerSec(void);

// Frame callback
AXAudioFrameCallback AXRegisterCallback(AXAudioFrameCallback cb);

// Volume
void AXSetMasterVolume(u16 vol);
u16  AXGetMasterVolume(void);

#ifdef __cplusplus
}
#endif
