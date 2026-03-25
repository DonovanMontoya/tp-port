#pragma once
/**
 * port/include/JSystem/JUtility/JUTFont.h
 * PC port shadow — keeps full class API; all GX rendering is stubbed.
 */
#include "JSystem/JUtility/TColor.h"
#include <cstring>

struct ResFONT {
    struct INF1 {
        u32 magic;
        u32 size;
        u16 fontType;
        u16 ascent;
        u16 descent;
        u16 width;
        u16 leading;
        u16 defaultCode;
    };
    struct WID1 {
        u32 magic;
        u32 size;
        u16 startCode;
        u16 endCode;
        u8  mChunkNum[4];
    };
    struct MAP1 {
        u32 magic;
        u32 size;
        u16 mappingMethod;
        u16 startCode;
        u16 endCode;
        u16 numEntries;
        u16 mLeading;
    };
    struct GLY1 {
        u32 magic;
        u32 size;
        u16 startCode;
        u16 endCode;
        u16 cellWidth;
        u16 cellHeight;
        u32 textureSize;
        u16 textureFormat;
        u16 numRows;
        u16 numColumns;
        u16 textureWidth;
        u16 textureHeight;
        u16 padding;
        u8  data[];
    };

    u64 magic;
    u32 filesize;
    u32 numBlocks;
    u8  padding[0x10];
    u8  data[];
};

class JUTFont {
public:
    JUTFont() : mValid(false), mFixed(false), mFixedWidth(0) {}
    virtual ~JUTFont() {}

    struct TWidth {
        u8 field_0x0;
        u8 field_0x1;
    };

    virtual void setGX() = 0;
    virtual void setGX(JUtility::TColor /*col1*/, JUtility::TColor /*col2*/) { setGX(); }
    virtual f32  drawChar_scale(f32, f32, f32, f32, int, bool) = 0;
    virtual int  getLeading()   const = 0;
    virtual s32  getAscent()    const = 0;
    virtual s32  getDescent()   const = 0;
    virtual s32  getHeight()    const = 0;
    virtual s32  getWidth()     const = 0;
    virtual void getWidthEntry(int i_no, TWidth* width) const = 0;
    virtual s32  getCellWidth()  const { return getWidth(); }
    virtual s32  getCellHeight() const { return getHeight(); }
    virtual int  getFontType()   const = 0;
    virtual ResFONT* getResFont() const = 0;
    virtual bool isLeadByte(int a1) const = 0;

    static bool isLeadByte_1Byte(int)   { return false; }
    static bool isLeadByte_2Byte(int)   { return true; }
    static bool isLeadByte_ShiftJIS(int b) {
        return (b >= 0x81 && b <= 0x9f) || (b >= 0xe0 && b <= 0xfc);
    }
    static bool isLeadByte_EUC(int b) {
        return (b >= 0xA1 && b <= 0xFE) || b == 0x8E;
    }

    void initialize_state() { mValid = false; }
    void setCharColor(JUtility::TColor col1)    { mColor1 = col1; }
    void setGradColor(JUtility::TColor col1, JUtility::TColor col2) { mColor3 = col1; mColor4 = col2; }

    f32 drawString_size_scale(f32 /*posX*/, f32 /*posY*/, f32 /*w*/, f32 /*h*/,
                               const char* /*str*/, u32 /*usz*/, bool /*visible*/) { return 0.0f; }

    void drawString(int posX, int posY, const char* str, bool visible) {
        drawString_size(posX, posY, str, strlen(str), visible);
    }
    s32 drawString_size(int posX, int posY, const char* str, u32 len, bool visible) {
        return (s32)drawString_size_scale((f32)posX, (f32)posY,
                                          (f32)getWidth(), (f32)getHeight(),
                                          str, len, visible);
    }
    f32 drawString_scale(f32 posX, f32 posY, f32 w, f32 h, const char* str, bool visible) {
        return (f32)(int)drawString_size_scale(posX, posY, w, h, str, strlen(str), visible);
    }

    int getWidth(int i_no) const {
        TWidth width;
        getWidthEntry(i_no, &width);
        return width.field_0x1;
    }
    int getOffset(int i_no) const {
        TWidth width;
        getWidthEntry(i_no, &width);
        return width.field_0x0;
    }

    bool isValid()        const { return mValid; }
    bool isFixed()        const { return mFixed; }
    int  getFixedWidth()  const { return mFixedWidth; }
    void setFixedWidth(bool fixed, int width) { mFixed = fixed; mFixedWidth = width; }

    bool              mValid;
    bool              mFixed;
    int               mFixedWidth;
    JUtility::TColor  mColor1;
    JUtility::TColor  mColor2;
    JUtility::TColor  mColor3;
    JUtility::TColor  mColor4;
};
