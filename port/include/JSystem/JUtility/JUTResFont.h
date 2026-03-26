#pragma once
/**
 * port/include/JSystem/JUtility/JUTResFont.h
 * PC port stub — JUTResFont (resource-based bitmap font).
 */
#include "JSystem/JUtility/JUTFont.h"

class JKRHeap;

class JUTResFont : public JUTFont {
public:
    JUTResFont(ResFONT const* /*font*/, JKRHeap* /*heap*/) {}
    JUTResFont(ResFONT& /*font*/, JKRHeap* /*heap*/) {}
    virtual ~JUTResFont() {}

    // JUTFont pure virtuals
    virtual void     setGX() {}
    virtual f32      drawChar_scale(f32, f32, f32, f32, int, bool) { return 0.0f; }
    virtual int      getLeading()   const { return 0; }
    virtual s32      getAscent()    const { return 0; }
    virtual s32      getDescent()   const { return 0; }
    virtual s32      getHeight()    const { return 0; }
    virtual s32      getWidth()     const { return 0; }
    virtual void     getWidthEntry(int, TWidth*) const {}
    virtual int      getFontType()  const { return 0; }
    virtual ResFONT* getResFont()   const { return nullptr; }
    virtual bool     isLeadByte(int) const { return false; }
};
