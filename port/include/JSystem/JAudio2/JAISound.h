#pragma once
/**
 * port/include/JSystem/JAudio2/JAISound.h
 * PC port stub — JAISound, JAISoundHandle, JAISoundID.
 */
#ifndef JAISOUND_H
#define JAISOUND_H

#include "port/types.h"
#include "JSystem/JAudio2/JAISoundID.h"

class JAISound {
public:
    JAISound() {}
    virtual ~JAISound() {}
    bool isPlaying() const { return false; }
    void stop(int) {}
};

class JAISoundHandle {
public:
    JAISoundHandle() : sound_(nullptr) {}
    ~JAISoundHandle() { releaseSound(); }
    bool  isSoundAttached() const  { return sound_ != nullptr; }
    JAISound* getSound()           { return sound_; }
    JAISound* operator->() const   { return sound_; }
    operator bool() const          { return isSoundAttached(); }
    void releaseSound()            { sound_ = nullptr; }
    JAISound* sound_;
};

#endif
