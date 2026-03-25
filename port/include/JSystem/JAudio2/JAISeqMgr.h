#pragma once
#ifndef JAISEQMGR_H
#define JAISEQMGR_H
#include "JSystem/JAudio2/JASGadget.h"
class JAISeqMgr : public JASGlobalInstance<JAISeqMgr> {
public:
    JAISeqMgr() : JASGlobalInstance<JAISeqMgr>(true) {}
};
#endif
