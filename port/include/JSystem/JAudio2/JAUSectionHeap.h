#pragma once
#ifndef JAUSECTIONHEAP_H
#define JAUSECTIONHEAP_H
#include "JSystem/JAudio2/JASGadget.h"
class JAUSectionHeap : public JASGlobalInstance<JAUSectionHeap> {
public:
    JAUSectionHeap() : JASGlobalInstance<JAUSectionHeap>(true) {}
};
#endif
