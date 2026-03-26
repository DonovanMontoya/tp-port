#pragma once
/**
 * port/include/m_Do/m_Do_audio.h
 * PC port stub — audio manager declarations without Z2Audio/JGeometry chain.
 *
 * The original pulls in Z2LinkMgr → Z2Creature → Z2SoundObject → JGeometry.
 * On PC all audio calls are no-ops.
 */
#ifndef M_DO_M_DO_AUDIO_H
#define M_DO_M_DO_AUDIO_H

#include "Z2AudioLib/Z2AudioMgr.h"
#include "port/types.h"
#include "JSystem/JKernel/JKRHeap.h"

// Forward declarations to avoid pulling in Z2SoundObject/JGeometry
struct Vec;

class mDoAud_zelAudio_c : public Z2AudioMgr {
public:
    mDoAud_zelAudio_c() : Z2AudioMgr() {}
    ~mDoAud_zelAudio_c() {}

    static void onInitFlag()   { mInitFlag  = true;  }
    static void onResetFlag()  { mResetFlag = true;  }
    static void offResetFlag() { mResetFlag = false; }
    static void onBgmSet()     { mBgmSet    = true;  }
    static void offBgmSet()    { mBgmSet    = false; }
    static u8   isInitFlag()   { return mInitFlag;   }
    static u8   isResetFlag()  { return mResetFlag;  }
    static u8   isBgmSet()     { return mBgmSet;     }

    static u8 mInitFlag;
    static u8 mResetFlag;
    static u8 mBgmSet;
};

extern JKRSolidHeap* g_mDoAud_audioHeap;

void mDoAud_Execute();
void mDoAud_resetProcess();
bool mDoAud_resetRecover();
void mDoAud_setSceneName(char const* spot, s32 room, s32 layer);
s32  mDoAud_load1stDynamicWave();
void mDoAud_setFadeInStart(u8 param_0);
void mDoAud_setFadeOutStart(u8 param_0);

// All sound/link calls are no-ops on PC ----------------------------------------
inline void mDoAud_seDeleteObject(Vec*)        {}
inline void mDoAud_seStart(u32, const Vec*, u32, s8)        {}
inline void mDoAud_seStartLevel(u32, const Vec*, u32, s8)   {}
inline void mDoAud_seStartCommon(u32, const Vec*)           {}
inline void mDoAud_bgmStart(u32)           {}
inline void mDoAud_bgmAllMute(u32)         {}
inline void mDoAud_subBgmStart(u32)        {}
inline void mDoAud_subBgmStop()            {}
inline void mDoAud_bgmNowBattle(f32)       {}
inline void mDoAud_bgmStreamPrepare(u32)   {}
inline void mDoAud_bgmStreamPlay()         {}
inline void mDoAud_setHour(s32)            {}
inline void mDoAud_setMinute(s32)          {}
inline void mDoAud_setWeekday(s32)         {}
inline void mDoAud_setInDarkness(bool)     {}
inline void mDoAud_setLinkGroupInfo(u8)    {}
inline void mDoAud_setLinkHp(s32, s32)     {}
inline void mDoAud_setLinkShieldType(s32, s32) {}
inline void mDoAud_bgmSetSwordUsing(s32)   {}
inline u32  mDoAud_checkPlayingSubBgmFlag(){ return 0; }
inline void mDoAud_setEnvSe(int, const Vec*, int, int, int, int) {}
inline int  mDoAud_monsSeStart(u32 /*soundId*/, const Vec* /*pos*/, u32 /*actorId*/,
                                u32 /*param*/, s8 /*reverb*/) { return 0; }

#endif /* M_DO_M_DO_AUDIO_H */
