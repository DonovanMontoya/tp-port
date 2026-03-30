#pragma once
/**
 * port/include/JSystem/J2DGraph/J2DPicture.h
 * PC port stub — J2DPane / J2DPicture 2D UI widget stubs.
 */
#ifndef PORT_J2DPICTURE_H
#define PORT_J2DPICTURE_H

#if defined(TP_PORT_USE_REAL_TITLE) && TP_PORT_USE_REAL_TITLE

#include "../../../../libs/JSystem/include/JSystem/J2DGraph/J2DPicture.h"

#else

#include "port/types.h"
#include "JSystem/JUtility/TColor.h"

class J2DPane {
public:
    virtual ~J2DPane() {}
    u8  getAlpha()  const { return 0xFF; }
    f32 getHeight() const { return 0.0f; }
    f32 getWidth()  const { return 0.0f; }
    void setAlpha(u8) {}
};

class J2DPicture : public J2DPane {
public:
    virtual ~J2DPicture() {}
    virtual void append(const char*, f32)  {}
    virtual void setBlendRatio(f32, f32)   {}
    void setWhite(JUtility::TColor)        {}
    void setBlack(JUtility::TColor)        {}
    bool changeTexture(const char*, u32)   { return false; }
    bool changeTexture(void*, u32)         { return false; }
};

#endif

#endif /* PORT_J2DPICTURE_H */
