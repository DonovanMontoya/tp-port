#pragma once
#ifndef JASDEFAULTBANKTABLE_H
#define JASDEFAULTBANKTABLE_H
#include "JSystem/JAudio2/JASGadget.h"
class JASDefaultBankTable : public JASGlobalInstance<JASDefaultBankTable> {
public:
    JASDefaultBankTable() : JASGlobalInstance<JASDefaultBankTable>(true) {}
};
#endif
