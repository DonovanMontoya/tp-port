/**
 * port/include/JSystem/JKernel/JKRDisposer.h
 * PC port stub — base class for objects allocated on a JKRHeap.
 */
#pragma once
#include "JSystem/JSupport/JSUList.h"

class JKRHeap;

class JKRDisposer {
public:
    JKRDisposer();
    virtual ~JKRDisposer();

    JKRHeap*             mHeap;
    JSULink<JKRDisposer> mLink;
};
