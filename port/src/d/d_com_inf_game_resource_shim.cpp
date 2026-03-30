#include "d/d_com_inf_game.h"

#include "d/d_resorce.h"

namespace {

constexpr ptrdiff_t kResControlOffset = 0x1C2F8;

static dRes_control_c* tpPortGetResControl() {
    auto* gameInfoBase = reinterpret_cast<unsigned char*>(&g_dComIfG_gameInfo);
    return reinterpret_cast<dRes_control_c*>(gameInfoBase + kResControlOffset);
}

}  // namespace

void tpPort_DumpResControlTag() {
    tpPortGetResControl()->dumpTag();
}

void tpPort_DumpResControlCsv(char* csv) {
    tpPortGetResControl()->dump(csv);
}
