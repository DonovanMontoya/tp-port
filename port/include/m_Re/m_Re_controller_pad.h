#ifndef M_RE_CONTROLLER_PAD_H
#define M_RE_CONTROLLER_PAD_H

#include "SSystem/SComponent/c_xyz.h"
#include "revolution/wpad.h"

#if PLATFORM_SHIELD
#define RECPD_SAMPLING_BUF_COUNT 16
#else
#define RECPD_SAMPLING_BUF_COUNT 10
#endif

struct KPADFSStickStatus {
    f32 x;
    f32 y;
};

struct Vec2 {
    f32 x;
    f32 y;
};

struct KPADFSStatus {
    KPADFSStickStatus stick;
};

struct KPADExStatus {
    KPADFSStatus fs;
};

struct KPADStatus {
    u32 hold;
    u32 trig;
    KPADExStatus ex_status;
};

class mReCPd {
public:
    struct Pad {
        KPADStatus field_0x4[RECPD_SAMPLING_BUF_COUNT];
        Vec2 m_stick3D;
        f32 m_stickValue;
        s16 m_stickAngle;
        Vec2 m_dpd_2d_pos;
        Vec2 m_dpd_ratio_pos;
        Vec2 m_dpd_play_box_pos;
        Vec2 m_swingVec;
        u32 m_swingBlock;
        u32 dev_type;
    };

    struct motorWave_t {
        u8* field_0x0;
        u16 field_0x4;
        u16 field_0x6;
        u8 field_0x8;
    };

    static void enableDpd(u32);
    static void disableDpd(u32);
    static void onConnect(u32);
    static void setExtensionCallback();
    static void onGetInfoAsync(u32);
    static void create();
    static void startMoterWave(motorWave_t*, u8*, bool);
    static void startMoterWave(int, u8*, bool);
    static void stopMoterWave(motorWave_t*);
    static void stopMoterWave(int);
    static void updateMoterWave();
    static void stopMoter();
    static BOOL getLowBat(u32 chan);
    static void setLowBat(u32 chan, BOOL low_bat);
    static void onLowBatChk(u32 chan);
    static void procNoData(u32 chan);
    static void read();
    static f32 calibrateDist(int);
    static Vec2& getDpd2DPos(u32 chan);
    static Vec2& getDpdRatioPos(u32 chan);
    static Vec2& getDpdPlayBoxPos(u32 chan);
    static BOOL chkDpdOk(u32 chan);
    static BOOL chkDpdValid(u32 chan);
    static BOOL chkDpdPosIn(u32 chan);
    static void getKPADAcc(u32 chan, Vec*, int);
    static s16 getAngleXy(u32 chan);
    static s16 getAngleZy(u32 chan);
    static f32 getKPADAccValue(u32 chan, int);
    static f32 getKPADAccValueMax(u32 chan);
    static f32 getKPADAccSpeed(u32 chan, int);
    static f32 getKPADAccSpeedMax(u32 chan);
    static f32 getKPADFSAccValue(u32 chan, int);
    static f32 getKPADFSAccValueMax(u32 chan);
    static s16 getFSStickAngle3DRev(u32 chan);
    static void calcUnderVec(void*, f32);

    static u32 getHoldB(u32);
    static u32 getHoldMinus(u32);
    static u32 getHoldPlus(u32);
    static u32 getHoldZ1(u32);
    static u32 getTrigB(u32);
    static u32 getTrigMinus(u32);
    static u32 getTrigHome(u32);
    static u32 getTrigPlus(u32);
    static u32 getTrigZ1(u32);
    static u32 chkSwingBlock(u32);
    static u32 getTrigSwing(u32);
    static Vec2& getSwingVec(u32);
    static u32 chkSimpleProbe(u32);
    static BOOL chkDevTypeValid(u32);
    static u32 getFSTrigSwing(u32);
    static void plusCalValue();
    static void minusCalValue();

    static WPADInfo& getPadInfo(u32 i_pad) { return m_pad_info[i_pad]; }
    static Pad& getPad(u32 i_pad) { return m_pad[i_pad]; }
    static u32 getTrig(u32 i_pad) { return getPad(i_pad).field_0x4[0].trig; }
    static u32 getHold(u32 i_pad) { return getPad(i_pad).field_0x4[0].hold; }

    static u32 getHoldA(u32 i_pad) { return getHold(i_pad) & WPAD_BUTTON_A; }
    static u32 getHoldRight(u32 i_pad) { return getHold(i_pad) & WPAD_BUTTON_RIGHT; }
    static u32 getHoldLeft(u32 i_pad) { return getHold(i_pad) & WPAD_BUTTON_LEFT; }
    static u32 getHoldUp(u32 i_pad) { return getHold(i_pad) & WPAD_BUTTON_UP; }
    static u32 getTrigA(u32 i_pad) { return getTrig(i_pad) & WPAD_BUTTON_A; }
    static u32 getTrigRight(u32 i_pad) { return getTrig(i_pad) & WPAD_BUTTON_RIGHT; }
    static u32 getTrigLeft(u32 i_pad) { return getTrig(i_pad) & WPAD_BUTTON_LEFT; }
    static u32 getTrigUp(u32 i_pad) { return getTrig(i_pad) & WPAD_BUTTON_UP; }
    static u32 getTrigStart(u32 i_pad) { return getTrig(i_pad) & WPAD_BUTTON_PLUS; }
    static f32 getStickX(u32 i_pad) { return getPad(i_pad).field_0x4[0].ex_status.fs.stick.x; }

    static WPADInfo m_pad_info[WPAD_MAX_CONTROLLERS];
    static Pad m_pad[WPAD_MAX_CONTROLLERS];
    static motorWave_t m_motorWave[WPAD_MAX_CONTROLLERS];
    static int m_cal_value;
};

#endif
