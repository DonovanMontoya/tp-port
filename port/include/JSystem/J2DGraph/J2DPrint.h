#pragma once
/**
 * port/include/JSystem/J2DGraph/J2DPrint.h
 * PC port stub — 2D text printer (wraps JUTFont).
 * All GX rendering is no-op; print() outputs via printf.
 */
#include "JSystem/JUtility/JUTFont.h"
#include "JSystem/JUtility/TColor.h"
#include "port/types.h"
#include <cstdarg>
#include <cstdio>

// Needed for binding enums
enum J2DTextBoxHBinding { J2DTEXT_H_LEFT = 0, J2DTEXT_H_CENTER = 1, J2DTEXT_H_RIGHT = 2 };
enum J2DTextBoxVBinding { J2DTEXT_V_TOP  = 0, J2DTEXT_V_CENTER = 1, J2DTEXT_V_BOTTOM = 2 };

class J2DPrint;

// C trampoline declared extern "C" in the original
extern "C" f32 J2DPrint_print_alpha_va(J2DPrint*, u8, const char*, va_list);

class J2DPrint {
public:
    struct TSize {
        f32 field_0x0;
        f32 field_0x4;
    };

    J2DPrint(JUTFont* font, JUtility::TColor c1, JUtility::TColor c2)
        : mFont(font), mCharColor(c1), mGradColor(c2)
        , mFontSizeX(8.0f), mFontSizeY(16.0f)
        , mScaleX(1.0f), mScaleY(1.0f)
        , mCursorH(0.0f), mCursorV(0.0f)
        , mCharSpacing(0.0f), mLineSpacing(0.0f)
    {}

    J2DPrint(JUTFont* font, f32 sx, f32 sy,
             JUtility::TColor c1, JUtility::TColor c2,
             JUtility::TColor c3, JUtility::TColor c4)
        : mFont(font), mCharColor(c3), mGradColor(c4)
        , mFontSizeX(sx), mFontSizeY(sy)
        , mScaleX(1.0f), mScaleY(1.0f)
        , mCursorH(0.0f), mCursorV(0.0f)
        , mCharSpacing(0.0f), mLineSpacing(0.0f)
    {}

    virtual ~J2DPrint() {}

    void  initiate()        {}
    void  setFontSize()     { if (mFont) { mFontSizeX = (f32)mFont->getWidth(); mFontSizeY = (f32)mFont->getHeight(); } }
    void  setFontSize(f32 x, f32 y) {
        mFontSizeX = (x > 0.0f) ? x : 0.0f;
        mFontSizeY = (y > 0.0f) ? y : 0.0f;
    }
    void  locate(f32 x, f32 y)  { mCursorH = x; mCursorV = y; }

    f32   print(f32 /*x*/, f32 /*y*/, u8 /*alpha*/, char const* fmt, ...) {
        va_list args; va_start(args, fmt); ::vprintf(fmt, args); va_end(args); return 0.0f;
    }
    void  printReturn(char const* /*str*/, f32, f32, J2DTextBoxHBinding, J2DTextBoxVBinding, f32, f32, u8) {}
    f32   parse(u8 const*, int, int, u16*, TSize&, u8, bool) { return 0.0f; }
    void  doCtrlCode(int) {}
    u16   doEscapeCode(u8 const**, u8) { return 0; }
    void  initchar()      {}
    s32   getNumberS32(u8 const**, s32 def, s32, int) { return def; }
    f32   getNumberF32(u8 const**, f32 def, f32, int) { return def; }

    f32   print_va(u8 alpha, const char* fmt, va_list args) {
        return J2DPrint_print_alpha_va(this, alpha, fmt, args);
    }

    static u8*    setBuffer(size_t sz) { (void)sz; return nullptr; }

    JUTFont*          getFont()    const { return mFont; }
    f32               getCursorV() const { return mCursorV; }
    void setCharColor(JUtility::TColor c) { mCharColor = c; }
    void setGradColor(JUtility::TColor c) { mGradColor = c; }

    static char*   mStrBuff;
    static size_t  mStrBuffSize;

private:
    JUTFont*          mFont;
    JUtility::TColor  field_0x8;
    JUtility::TColor  field_0xc;
    f32               field_0x10;
    f32               field_0x14;
    f32               mScaleX;
    f32               mScaleY;
    s16               field_0x20;
    bool              field_0x22;
    f32               field_0x24;
    f32               field_0x28;
    f32               mCursorH;
    f32               mCursorV;
    f32               field_0x34;
    JUtility::TColor  mBlackColor;
    JUtility::TColor  mWhiteColor;
    JUtility::TColor  mCharColor;
    JUtility::TColor  mGradColor;
    f32               mCharSpacing;
    f32               mLineSpacing;
    f32               mFontSizeX;
    f32               mFontSizeY;
    s16               field_0x58;
    bool              field_0x5a;
};
