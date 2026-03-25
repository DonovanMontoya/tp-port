#pragma once
#ifndef Z2SEQMGR_H
#define Z2SEQMGR_H
#include "JSystem/JAudio2/JASGadget.h"
class Z2SeqMgr : public JASGlobalInstance<Z2SeqMgr> {
public:
    Z2SeqMgr() : JASGlobalInstance<Z2SeqMgr>(true) {}
};
#endif
