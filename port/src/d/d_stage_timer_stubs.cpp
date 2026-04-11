#include "d/d_stage.h"
class J2DScreen;
class J2DAnmTransform;
#include "d/d_timer.h"

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
