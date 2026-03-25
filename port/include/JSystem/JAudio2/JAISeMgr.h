#pragma once
#ifndef JAISEMGR_H
#define JAISEMGR_H
#include "JSystem/JAudio2/JASGadget.h"
class JAISeMgr : public JASGlobalInstance<JAISeMgr> {
public:
    JAISeMgr() : JASGlobalInstance<JAISeMgr>(true) {}
};
#endif
