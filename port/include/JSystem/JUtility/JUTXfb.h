#pragma once
#ifndef JUTXFB_H
#define JUTXFB_H

#include "JSystem/JUtility/JUTVideo.h"

class JUTXfb {
public:
    enum EXfbNumber {
        UNK_2 = 2,
    };

    static JUTXfb* getManager() { return sManager; }
    void* getDisplayingXfb() const { return nullptr; }
    void clearIndex() {}
    void setDrawingXfbIndex(int /*index*/) {}

    inline static JUTXfb* sManager = nullptr;
};

#endif /* JUTXFB_H */
