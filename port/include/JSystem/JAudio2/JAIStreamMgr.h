#pragma once
#ifndef JAISTREAMMANAGER_H
#define JAISTREAMMANAGER_H
#include "JSystem/JAudio2/JASGadget.h"
class JAIStreamMgr : public JASGlobalInstance<JAIStreamMgr> {
public:
    JAIStreamMgr() : JASGlobalInstance<JAIStreamMgr>(true) {}
};
#endif
