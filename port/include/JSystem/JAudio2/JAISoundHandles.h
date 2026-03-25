#pragma once
/**
 * port/include/JSystem/JAudio2/JAISoundHandles.h
 * PC port stub.
 */
#ifndef JAISOUNDHANDLES_H
#define JAISOUNDHANDLES_H

#include "JSystem/JAudio2/JAISound.h"
#include "JSystem/JAudio2/JAISoundID.h"

class JAISoundHandles {
public:
    JAISoundHandles(JAISoundHandle* handle, int handleNum)
        : handle_(handle), numHandles_(handleNum) {}
    JAISoundHandle& operator[](int n) { return handle_[n]; }
    JAISoundHandle* getHandleSoundID(JAISoundID /*id*/) { return nullptr; }
    JAISoundHandle* getFreeHandle() { return nullptr; }
private:
    JAISoundHandle* handle_;
    int             numHandles_;
};

#endif
