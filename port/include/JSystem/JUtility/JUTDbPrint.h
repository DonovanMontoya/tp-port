#pragma once
/**
 * port/include/JSystem/JUtility/JUTDbPrint.h
 * PC port stub — GameCube debug print overlay (writes to framebuffer directly).
 * All methods are no-ops on PC.
 */
#include "port/types.h"

class JKRHeap;

// Standalone setup function referenced by some game files
inline void JUTDbPrint_SetupTexture() {}

class JUTDbPrint {
public:
    JUTDbPrint() {}
    virtual ~JUTDbPrint() {}

    static JUTDbPrint* create(JKRHeap* /*heap*/ = nullptr) {
        static JUTDbPrint s_instance;
        return &s_instance;
    }

    void  print(const char* /*fmt*/, ...) {}
    void  printf(const char* /*fmt*/, ...) {}
    void  locate(int /*x*/, int /*y*/) {}
    void  setTextColor(u32 /*color*/) {}
    void  setBackColor(u32 /*color*/) {}
    void  eraseLine() {}
    void  eraseScreen() {}
    void  draw() {}
    void  flush() {}

    static JUTDbPrint* getManager() {
        static JUTDbPrint s_instance;
        return &s_instance;
    }
};
