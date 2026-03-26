#pragma once
/**
 * port/include/JSystem/JUtility/JUTTexture.h
 * PC port stub — GX texture wrapper; no GX on PC so all methods are no-ops.
 */
#include "port/types.h"

class JKRHeap;
struct ResTIMG {
    u8 format = 0;
    u8 alphaEnabled = 0;
    u16 width = 0;
    u16 height = 0;
    u8 wrapS = 0;
    u8 wrapT = 0;
    u8 indexTexture = 0;
    u8 colorFormat = 0;
    u16 numColors = 0;
    uintptr_t paletteOffset = 0;
    u8 mipmapEnabled = 0;
    u8 doEdgeLOD = 0;
    u8 biasClamp = 0;
    u8 maxAnisotropy = 0;
    u8 minFilter = 0;
    u8 magFilter = 0;
    s8 minLOD = 0;
    s8 maxLOD = 0;
    u8 mipmapCount = 0;
    u8 unknown = 0;
    s16 LODBias = 0;
    uintptr_t imageOffset = 0;
};

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
