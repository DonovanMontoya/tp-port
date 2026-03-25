/**
 * src/dolphin/ax.cpp
 * AX → OpenAL audio implementation.
 *
 * AX is the GameCube's DSP audio mixer. It operates on 32 kHz PCM at
 * 5 ms frames (160 samples/frame). Each active voice references ARAM
 * (audio RAM) samples; we substitute host-memory pointers.
 *
 * ADPCM decoding is required for most in-game sounds (GC ADPCM / DSP-ADPCM).
 */

#include "port/port.h"
#include "dolphin/ax.h"
#include "port/endian.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <cstring>
#include <cmath>
#include <vector>
#include <array>
#include <mutex>
#include <thread>
#include <atomic>

// -----------------------------------------------------------------------
// OpenAL context
// -----------------------------------------------------------------------
static ALCdevice*  sALDevice  = nullptr;
static ALCcontext* sALContext = nullptr;

// -----------------------------------------------------------------------
// Voice pool
// -----------------------------------------------------------------------
struct Voice {
    bool          inUse     = false;
    u16           state     = AX_PB_STATE_STOP;
    AXUserCallback userCb   = nullptr;
    u32           cbData    = 0;
    ALuint        alSource  = 0;
    ALuint        alBuffer  = 0;
    AXPB          pb        = {};
};

static std::array<Voice, AX_MAX_VOICES> sVoices;
static std::mutex sVoiceMutex;

// -----------------------------------------------------------------------
// Audio frame callback
// -----------------------------------------------------------------------
static AXAudioFrameCallback sFrameCB = nullptr;
static std::thread          sFrameThread;
static std::atomic<bool>    sFrameRunning{false};
static u16                  sMasterVol = 0x7FFF;

// Frame thread: fires at ~200 Hz (5 ms intervals) to match GC AX timing
static void FrameWorker() {
    using namespace std::chrono_literals;
    while (sFrameRunning) {
        std::this_thread::sleep_for(5ms);
        if (sFrameCB) sFrameCB();
    }
}

// -----------------------------------------------------------------------
// GC DSP-ADPCM decoder (simplified)
// Decodes one GC ADPCM block (14 nibbles → 14 samples).
// -----------------------------------------------------------------------
static void DecodeADPCMBlock(const uint8_t* src, int16_t* dst,
                              const int16_t coef[8][2],
                              int16_t& yn1, int16_t& yn2)
{
    uint8_t header  = src[0];
    int     scale   = 1 << (header & 0x0F);
    int     coefIdx = (header >> 4) & 0x07;
    int32_t c0 = coef[coefIdx][0];
    int32_t c1 = coef[coefIdx][1];

    for (int i = 0; i < 14; i++) {
        int nibble = (i & 1) ? (src[1 + i/2] & 0x0F) : (src[1 + i/2] >> 4);
        if (nibble >= 8) nibble -= 16;
        int32_t sample = (((int32_t)nibble * scale) << 11) + 1024;
        sample = (sample + c0 * yn1 + c1 * yn2) >> 11;
        if (sample >  32767) sample =  32767;
        if (sample < -32768) sample = -32768;
        dst[i] = (int16_t)sample;
        yn2 = yn1;
        yn1 = (int16_t)sample;
    }
}

// Convert entire ADPCM stream to PCM16 for OpenAL upload
static std::vector<int16_t> DecodeADPCM(const uint8_t* data, uint32_t addrEnd,
                                          uint32_t addrStart,
                                          const int16_t coef[8][2],
                                          int16_t yn1, int16_t yn2)
{
    uint32_t samples = (addrEnd - addrStart) / 8 * 14;
    std::vector<int16_t> out;
    out.reserve(samples);

    uint32_t blockCount = (addrEnd - addrStart + 7) / 8;
    for (uint32_t b = 0; b < blockCount; b++) {
        int16_t decoded[14];
        DecodeADPCMBlock(data + b * 8, decoded, coef, yn1, yn2);
        for (int16_t s : decoded) out.push_back(s);
    }
    return out;
}

// -----------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------
void AXInit(void) {
    sALDevice  = alcOpenDevice(nullptr);
    sALContext = alcCreateContext(sALDevice, nullptr);
    alcMakeContextCurrent(sALContext);

    // Create AL sources for all voices
    for (auto& v : sVoices) {
        alGenSources(1, &v.alSource);
        alGenBuffers(1, &v.alBuffer);
    }

    sFrameRunning = true;
    sFrameThread  = std::thread(FrameWorker);
    tp::log::info("AXInit complete (OpenAL %s)", alGetString(AL_VERSION));
}

void AXQuit(void) {
    sFrameRunning = false;
    if (sFrameThread.joinable()) sFrameThread.join();

    for (auto& v : sVoices) {
        if (v.alSource) { alDeleteSources(1, &v.alSource); v.alSource = 0; }
        if (v.alBuffer) { alDeleteBuffers(1, &v.alBuffer); v.alBuffer = 0; }
    }
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(sALContext);
    alcCloseDevice(sALDevice);
}

AXPB* AXAcquireVoice(u32 priority, AXUserCallback cb, u32 cbData) {
    std::lock_guard<std::mutex> lock(sVoiceMutex);
    for (auto& v : sVoices) {
        if (!v.inUse) {
            v.inUse  = true;
            v.userCb = cb;
            v.cbData = cbData;
            v.state  = AX_PB_STATE_STOP;
            memset(&v.pb, 0, sizeof(v.pb));
            v.pb.priority  = (u16)priority;
            v.pb._alSource = v.alSource;
            alSourceStop(v.alSource);
            return &v.pb;
        }
    }
    return nullptr;  // pool exhausted
}

void AXFreeVoice(AXPB* pb) {
    if (!pb) return;
    std::lock_guard<std::mutex> lock(sVoiceMutex);
    for (auto& v : sVoices) {
        if (&v.pb == pb) {
            alSourceStop(v.alSource);
            v.inUse = false;
            return;
        }
    }
}

void AXSetVoiceState(AXPB* pb, u16 state) {
    if (!pb) return;
    pb->state = state;
    ALuint src = pb->_alSource;
    if (state == AX_PB_STATE_RUN)  alSourcePlay(src);
    else                            alSourceStop(src);
}

void AXSetVoiceAddr(AXPB* pb, const AXPBADDR* addr) {
    if (!pb || !addr) return;
    memcpy(&pb->addr, addr, sizeof(AXPBADDR));
}

void AXSetVoiceAdpcm(AXPB* pb, const AXPBADPCM* adpcm) {
    if (!pb || !adpcm) return;
    memcpy(&pb->adpcm, adpcm, sizeof(AXPBADPCM));
}

void AXSetVoiceSrc(AXPB* pb, const AXPBSRC* src) {
    if (!pb || !src) return;
    memcpy(&pb->src, src, sizeof(AXPBSRC));
}

void AXSetVoiceSrcType(AXPB* pb, u32 /*type*/) { (void)pb; }

void AXSetVoiceSrcRatio(AXPB* pb, f32 ratio) {
    if (!pb) return;
    alSourcef(pb->_alSource, AL_PITCH, ratio);
}

void AXSetVoiceMix(AXPB* pb, const AXPBMIX* mix) {
    if (!pb || !mix) return;
    memcpy(&pb->mix, mix, sizeof(AXPBMIX));
    float vol = mix->vL / (float)0x7FFF;
    alSourcef(pb->_alSource, AL_GAIN, vol);
}

void AXSetVoiceVe(AXPB* pb, const AXPBVE* ve) {
    if (!pb || !ve) return;
    memcpy(&pb->ve, ve, sizeof(AXPBVE));
    float vol = ve->currentVolume / (float)0x7FFF;
    alSourcef(pb->_alSource, AL_GAIN, vol);
}

void AXSetVoiceAdpcmData(AXPB* pb, u16 predScale, s16 yn1, s16 yn2) {
    if (!pb) return;
    pb->adpcm.pred_scale = predScale;
    pb->adpcm.yn1 = yn1;
    pb->adpcm.yn2 = yn2;
}

void AXSetVoiceLoop(AXPB* pb, u16 flag) {
    if (!pb) return;
    pb->addr.loopFlag = flag;
    alSourcei(pb->_alSource, AL_LOOPING, flag ? AL_TRUE : AL_FALSE);
}

void AXSetVoiceLoopAddr(AXPB* pb, u32 addr) {
    if (pb) { pb->addr.loopAddrHi = addr >> 16; pb->addr.loopAddrLo = addr & 0xFFFF; }
}
void AXSetVoiceEndAddr(AXPB* pb, u32 addr) {
    if (pb) { pb->addr.endAddrHi = addr >> 16; pb->addr.endAddrLo = addr & 0xFFFF; }
}
void AXSetVoiceCurrentAddr(AXPB* pb, u32 addr) {
    if (pb) { pb->addr.curAddrHi = addr >> 16; pb->addr.curAddrLo = addr & 0xFFFF; }
}
u32 AXGetVoiceCurrentAddr(AXPB* pb) {
    if (!pb) return 0;
    return ((u32)pb->addr.curAddrHi << 16) | pb->addr.curAddrLo;
}

BOOL AXIsVoiceRunning(AXPB* pb) {
    if (!pb) return FALSE;
    ALint state = AL_STOPPED;
    alGetSourcei(pb->_alSource, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING) ? TRUE : FALSE;
}

u32  AXGetInputSamplesPerFrame(void) { return 160; }
u32  AXGetInputSamplesPerSec(void)   { return 32000; }

AXAudioFrameCallback AXRegisterCallback(AXAudioFrameCallback cb) {
    AXAudioFrameCallback old = sFrameCB;
    sFrameCB = cb;
    return old;
}

void AXSetMasterVolume(u16 vol) {
    sMasterVol = vol;
    alListenerf(AL_GAIN, vol / (float)0x7FFF);
}
u16 AXGetMasterVolume(void) { return sMasterVol; }
