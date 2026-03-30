#include "m_Re/m_Re_controller_pad.h"

#include <cstring>

WPADInfo mReCPd::m_pad_info[WPAD_MAX_CONTROLLERS] = {};
mReCPd::Pad mReCPd::m_pad[WPAD_MAX_CONTROLLERS] = {};
mReCPd::motorWave_t mReCPd::m_motorWave[WPAD_MAX_CONTROLLERS] = {};
int mReCPd::m_cal_value = 0;

void mReCPd::enableDpd(u32) {}
void mReCPd::disableDpd(u32) {}
void mReCPd::onConnect(u32) {}
void mReCPd::setExtensionCallback() {}
void mReCPd::onGetInfoAsync(u32) {}
void mReCPd::create() {
    std::memset(m_pad, 0, sizeof(m_pad));
    std::memset(m_pad_info, 0, sizeof(m_pad_info));
    std::memset(m_motorWave, 0, sizeof(m_motorWave));
}
void mReCPd::startMoterWave(motorWave_t*, u8*, bool) {}
void mReCPd::startMoterWave(int, u8*, bool) {}
void mReCPd::stopMoterWave(motorWave_t*) {}
void mReCPd::stopMoterWave(int) {}
void mReCPd::updateMoterWave() {}
void mReCPd::stopMoter() {}
BOOL mReCPd::getLowBat(u32 chan) { return m_pad_info[chan].lowBat; }
void mReCPd::setLowBat(u32 chan, BOOL low_bat) { m_pad_info[chan].lowBat = low_bat; }
void mReCPd::onLowBatChk(u32) {}
void mReCPd::procNoData(u32) {}
void mReCPd::read() {}
f32 mReCPd::calibrateDist(int) { return 0.0f; }
Vec2& mReCPd::getDpd2DPos(u32 chan) { return m_pad[chan].m_dpd_2d_pos; }
Vec2& mReCPd::getDpdRatioPos(u32 chan) { return m_pad[chan].m_dpd_ratio_pos; }
Vec2& mReCPd::getDpdPlayBoxPos(u32 chan) { return m_pad[chan].m_dpd_play_box_pos; }
BOOL mReCPd::chkDpdOk(u32) { return FALSE; }
BOOL mReCPd::chkDpdValid(u32) { return FALSE; }
BOOL mReCPd::chkDpdPosIn(u32) { return FALSE; }
void mReCPd::getKPADAcc(u32, Vec* vec, int count) {
    for (int i = 0; i < count; ++i) {
        vec[i].x = 0.0f;
        vec[i].y = 0.0f;
        vec[i].z = 0.0f;
    }
}
s16 mReCPd::getAngleXy(u32) { return 0; }
s16 mReCPd::getAngleZy(u32) { return 0; }
f32 mReCPd::getKPADAccValue(u32, int) { return 0.0f; }
f32 mReCPd::getKPADAccValueMax(u32) { return 0.0f; }
f32 mReCPd::getKPADAccSpeed(u32, int) { return 0.0f; }
f32 mReCPd::getKPADAccSpeedMax(u32) { return 0.0f; }
f32 mReCPd::getKPADFSAccValue(u32, int) { return 0.0f; }
f32 mReCPd::getKPADFSAccValueMax(u32) { return 0.0f; }
s16 mReCPd::getFSStickAngle3DRev(u32) { return 0; }
void mReCPd::calcUnderVec(void*, f32) {}
u32 mReCPd::getHoldB(u32 i_padNo) { return getHold(i_padNo) & WPAD_BUTTON_B; }
u32 mReCPd::getHoldMinus(u32 i_padNo) { return getHold(i_padNo) & WPAD_BUTTON_MINUS; }
u32 mReCPd::getHoldPlus(u32 i_padNo) { return getHold(i_padNo) & WPAD_BUTTON_PLUS; }
u32 mReCPd::getHoldZ1(u32 i_padNo) { return getHold(i_padNo) & WPAD_BUTTON_Z; }
u32 mReCPd::getTrigB(u32 i_padNo) { return getTrig(i_padNo) & WPAD_BUTTON_B; }
u32 mReCPd::getTrigMinus(u32 i_padNo) { return getTrig(i_padNo) & WPAD_BUTTON_MINUS; }
u32 mReCPd::getTrigHome(u32 i_padNo) { return getTrig(i_padNo) & WPAD_BUTTON_HOME; }
u32 mReCPd::getTrigPlus(u32 i_padNo) { return getTrig(i_padNo) & WPAD_BUTTON_PLUS; }
u32 mReCPd::getTrigZ1(u32 i_padNo) { return getTrig(i_padNo) & WPAD_BUTTON_Z; }
u32 mReCPd::chkSwingBlock(u32 i_padNo) { return m_pad[i_padNo].m_swingBlock; }
u32 mReCPd::getTrigSwing(u32) { return 0; }
Vec2& mReCPd::getSwingVec(u32 i_padNo) { return m_pad[i_padNo].m_swingVec; }
u32 mReCPd::chkSimpleProbe(u32 i_padNo) { return m_pad[i_padNo].dev_type != 0; }
BOOL mReCPd::chkDevTypeValid(u32 i_padNo) { return m_pad[i_padNo].dev_type != 0; }
u32 mReCPd::getFSTrigSwing(u32) { return 0; }
void mReCPd::plusCalValue() { ++m_cal_value; }
void mReCPd::minusCalValue() { --m_cal_value; }
