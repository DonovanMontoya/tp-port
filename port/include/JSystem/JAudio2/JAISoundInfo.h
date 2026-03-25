#pragma once
#ifndef JAISOUNDINFO_H
#define JAISOUNDINFO_H
#include "JSystem/JAudio2/JASGadget.h"
class JAISoundInfo : public JASGlobalInstance<JAISoundInfo> {
public:
    JAISoundInfo() : JASGlobalInstance<JAISoundInfo>(true) {}
};
#endif
