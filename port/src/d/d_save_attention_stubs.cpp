#include "d/d_attention.h"
#include "d/d_meter2_info.h"
#include "d/d_save_init.h"

void setInitEventBit() {}

void dMeter2Info_c::init() {}
void dMeter2Info_c::setHotSpringTimer(u8) {}
void dMeter2Info_c::setSaveStageName(const char*) {}

dAttParam_c::~dAttParam_c() = default;
dAttention_c::~dAttention_c() = default;
int dAttDraw_CallBack_c::execute(u16, J3DTransformInfo*) {
    return 1;
}
