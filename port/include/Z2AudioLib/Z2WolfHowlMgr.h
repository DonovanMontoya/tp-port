#pragma once
/**
 * port/include/Z2AudioLib/Z2WolfHowlMgr.h
 * PC port stub — Z2WolfHowlMgr audio manager.
 * Only the class name and JASGlobalInstance base are needed here.
 */
#ifndef Z2WOLFHOWLMGR_H
#define Z2WOLFHOWLMGR_H

// Pull in all JAudio2 / Z2AudioLib type stubs needed for the
// JASGlobalInstance<T>::sInstance specialisations at the bottom of m_Do_main.cpp.
#include "JSystem/JAudio2/JASGadget.h"
#include "JSystem/JAudio2/JAUSoundInfo.h"
#include "JSystem/JAudio2/JAISoundInfo.h"
#include "JSystem/JAudio2/JAIStreamMgr.h"
#include "JSystem/JAudio2/JAISeqMgr.h"
#include "JSystem/JAudio2/JAISeMgr.h"
#include "JSystem/JAudio2/JAISoundStarter.h"
#include "JSystem/JAudio2/JASDefaultBankTable.h"
#include "JSystem/JAudio2/JAUSectionHeap.h"
#include "Z2AudioLib/Z2EnvSeMgr.h"
#include "Z2AudioLib/Z2FxLineMgr.h"
#include "Z2AudioLib/Z2Audience.h"
#include "Z2AudioLib/Z2SoundObjMgr.h"
#include "Z2AudioLib/Z2SoundInfo.h"
#include "Z2AudioLib/Z2SoundMgr.h"
#include "Z2AudioLib/Z2SpeechMgr2.h"
#include "Z2AudioLib/Z2SoundStarter.h"
#include "Z2AudioLib/Z2StatusMgr.h"
#include "Z2AudioLib/Z2SceneMgr.h"
#include "Z2AudioLib/Z2SeqMgr.h"
#include "Z2AudioLib/Z2SeMgr.h"

// Forward declarations for types used in Z2WolfHowlMgr
struct Z2WolfHowlLine { u8 type; u8 length; };
struct Z2WolfHowlData { u8 lineNum; Z2WolfHowlLine* lineData; };

enum Z2WolfHowlCurveID {
    Z2WOLFHOWL_NONE = -1,
    Z2WOLFHOWL_TOBIKUSA, Z2WOLFHOWL_UMAKUSA, Z2WOLFHOWL_ZELDASONG,
    Z2WOLFHOWL_HEALINGSONG, Z2WOLFHOWL_SOULREQUIEM, Z2WOLFHOWL_LIGHTPRELUDE,
    Z2WOLFHOWL_NEWSONG1, Z2WOLFHOWL_NEWSONG2, Z2WOLFHOWL_NEWSONG3,
    Z2WOLFHOWL_MAX
};

class Z2WolfHowlMgr : public JASGlobalInstance<Z2WolfHowlMgr> {
public:
    Z2WolfHowlMgr() : JASGlobalInstance<Z2WolfHowlMgr>(true) {}

    Z2WolfHowlLine getCorrectLine(u8) { return Z2WolfHowlLine{0, 0}; }
    u8 getCorrectLineNum() { return 0; }
    s8 getOnLineNum() { return -1; }
    void setCorrectCurve(int) {}
    void startWindStoneSound(s8, Vec*) {}
    s16 startWolfHowlSound(f32, f32, bool, f32) { return 0; }
    void skipCorrectDemo() {}
    s32 getReleaseTimer() { return 0; }
    u32 getTimer() { return 0; }
    u8 getCorrectCurveID() { return 0; }
};

#endif /* Z2WOLFHOWLMGR_H */
