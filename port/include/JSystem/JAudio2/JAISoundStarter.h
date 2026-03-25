#pragma once
#ifndef JAISOUNDSTARTER_H
#define JAISOUNDSTARTER_H
#include "JSystem/JAudio2/JASGadget.h"
class JAISoundStarter : public JASGlobalInstance<JAISoundStarter> {
public:
    JAISoundStarter() : JASGlobalInstance<JAISoundStarter>(true) {}
};
#endif
