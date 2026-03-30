#include "d/d_stage.h"
class J2DScreen;
class J2DAnmTransform;
#include "d/d_timer.h"
#include <cstring>

JKRExpHeap* dStage_roomControl_c::mMemoryBlock[dStage_roomControl_c::MEMORY_BLOCK_MAX] = {};
char dStage_roomControl_c::mArcBank[32][10] = {};
dStage_roomStatus_c dStage_roomControl_c::mStatus[0x40] = {};
char dStage_roomControl_c::mDemoArcName[10] = {};
fpc_ProcID dStage_roomControl_c::mProcID = fpcM_ERROR_PROCESS_ID_e;
dStage_roomControl_c::dStage_bankName* dStage_roomControl_c::mArcBankName = nullptr;
dStage_roomControl_c::dStage_bankData* dStage_roomControl_c::mArcBankData = nullptr;
dStage_roomControl_c::roomDzs_c dStage_roomControl_c::m_roomDzs = {};
s8 dStage_roomControl_c::mStayNo = 0;
s8 dStage_roomControl_c::mOldStayNo = 0;
s8 dStage_roomControl_c::mNextStayNo = 0;
u8 dStage_roomControl_c::m_time_pass = 0;
u8 dStage_roomControl_c::mNoChangeRoom = 0;
s8 dStage_roomControl_c::mRoomReadId = -1;

void dStage_stageDt_c::init() {}

stage_map_info_class* dStage_stageDt_c::getMapInfo2(int) const {
    return mMapInfo;
}

dStage_Elst_c* dStage_stageDt_c::getElst() {
    return mElst;
}

void dStage_roomDt_c::init() {}
void dStage_roomDt_c::initFileList2() {}
stage_map_info_class* dStage_roomDt_c::getMapInfo2(int) const {
    return mMapInfo;
}

void dStage_SetErrorStage() {}
void dStage_SetErrorRoom() {}

void dStage_roomControl_c::initZone() {}

dStage_roomDt_c* dStage_roomControl_c::getStatusRoomDt(int roomNo) {
    if (roomNo < 0 || roomNo >= 0x40) {
        return nullptr;
    }
    return &mStatus[roomNo].mRoomDt;
}

void dStage_startStage_c::set(const char* name, s8 roomNo, s16 point, s8 layer) {
    std::memset(mName, 0, sizeof(mName));
    if (name != nullptr) {
        std::strncpy(mName, name, sizeof(mName) - 1);
    }
    mRoomNo = roomNo;
    mPoint = point;
    mLayer = layer;
    mDarkArea = 0;
}

void dStage_nextStage_c::set(const char* name, s8 roomNo, s16 point, s8 layer, s8 darkArea,
                             u8 wipeSpeed) {
    dStage_startStage_c::set(name, roomNo, point, layer);
    setDarkArea(darkArea);
    enabled = 1;
    wipe = 0;
    wipe_speed = wipeSpeed;
}

const char* dStage_getName(s16, s8) {
    return "stage";
}

const char* dStage_getName2(s16, s8) {
    return "stage";
}

dStage_objectNameInf* dStage_searchName(const char*) {
    return nullptr;
}

s8 dStage_roomRead_dt_c_GetReverbStage(roomRead_class&, int) {
    return 0;
}

dDlst_TimerScrnDraw_c::dDlst_TimerScrnDraw_c() = default;
dDlst_TimerScrnDraw_c::~dDlst_TimerScrnDraw_c() = default;
void dDlst_TimerScrnDraw_c::draw() {}

s32 dTimer_createStockTimer() { return 0; }
fpc_ProcID dTimer_createTimer(s32, u32, u8, u8, f32, f32, f32, f32) {
    return fpcM_ERROR_PROCESS_ID_e;
}
u32 dTimer_createGetIn2D(s32, cXyz) { return 0; }
int dTimer_getRestTimeMs() { return 0; }
bool dTimer_isStart() { return false; }
void dTimer_show() {}
void dTimer_hide() {}
u8 dTimer_isReadyFlag() { return 1; }

int dTimer_c::_create() { return 1; }
int dTimer_c::_execute() { return 1; }
int dTimer_c::_draw() { return 1; }
int dTimer_c::_delete() { return 1; }
int dTimer_c::deleteCheck() { return 0; }
int dTimer_c::start(int, s16) { return 1; }
int dTimer_c::start(int) { return 1; }
int dTimer_c::stock_start(s16) { return 1; }
bool dTimer_c::stock_start() { return true; }
int dTimer_c::stop(u8) { return 1; }
int dTimer_c::restart(u8) { return 1; }
int dTimer_c::end(int) { return 1; }
int dTimer_c::deleteRequest() { return 1; }
int dTimer_c::getTimeMs() { return 0; }
int dTimer_c::getLimitTimeMs() { return 0; }
int dTimer_c::getRestTimeMs() { return 0; }
bool dTimer_c::isStart() { return false; }
int dTimer_c::createGetIn(cXyz) { return 1; }
