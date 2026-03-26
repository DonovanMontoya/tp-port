#pragma once
/**
 * port/include/JSystem/JUtility/JUTCacheFont.h
 * PC port stub — JUTCacheFont (Glyph-cache bitmap font).
 * Inherits JUTFont and provides no-op implementations; no GX rendering on PC.
 */
#include "JSystem/JUtility/JUTFont.h"

class JKRHeap;

class JUTCacheFont : public JUTFont {
public:
    enum EPagingType { PAGE_TYPE_0, PAGE_TYPE_1 };

    JUTCacheFont(ResFONT const* /*font*/, u32 /*cacheSize*/, JKRHeap* /*heap*/) {}
    virtual ~JUTCacheFont() {}

    static u32 calcCacheSize(u32 /*a*/, u32 /*b*/) { return 0; }
    void setPagingType(EPagingType /*type*/) {}
    int  getCachePage()     const { return 0; }
    int  getMaxSheetSize()  const { return 0; }

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
