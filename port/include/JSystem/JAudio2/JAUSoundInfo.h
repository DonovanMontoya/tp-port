#pragma once
#ifndef JAUSOUNDINFO_H
#define JAUSOUNDINFO_H
#include "JSystem/JAudio2/JASGadget.h"
class JAUSoundInfo : public JASGlobalInstance<JAUSoundInfo> {
public:
    JAUSoundInfo() : JASGlobalInstance<JAUSoundInfo>(true) {}
};
#endif
