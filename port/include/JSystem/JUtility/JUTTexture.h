#pragma once
/**
 * port/include/JSystem/JUtility/JUTTexture.h
 * PC port stub — GX texture wrapper; no GX on PC so all methods are no-ops.
 */
#include "port/types.h"

class JKRHeap;

class JUTTexture {
public:
    JUTTexture()  {}
    virtual ~JUTTexture() {}

    // Load from file / memory — no-op on PC
    bool load(const char* /*path*/, JKRHeap* /*heap*/ = nullptr) { return false; }
    bool load(const void* /*data*/, u32 /*size*/)                 { return false; }

    // Drawing — no-op on PC
    void draw(f32 /*x*/, f32 /*y*/, f32 /*w*/, f32 /*h*/) const {}
    void draw(f32 /*x*/, f32 /*y*/)                        const {}

    // Accessors that return safe defaults
    u16  getWidth()  const { return 0; }
    u16  getHeight() const { return 0; }
    bool isLoaded()  const { return false; }
};
