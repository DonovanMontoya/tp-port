#pragma once
#ifndef JUTXFB_H
#define JUTXFB_H

#include "JSystem/JUtility/JUTVideo.h"

class JUTXfb {
public:
    static JUTXfb* getManager() { return sManager; }
    void* getDisplayingXfb() const { return nullptr; }
    void clearIndex() {}

    inline static JUTXfb* sManager = nullptr;
};

#endif /* JUTXFB_H */
