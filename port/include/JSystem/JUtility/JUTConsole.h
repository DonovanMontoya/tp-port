#pragma once
/**
 * port/include/JSystem/JUtility/JUTConsole.h
 * PC port shadow of JUTConsole.
 * print/print_f backed by printf; all GX drawing is a no-op.
 */
#include "JSystem/JGadget/linklist.h"
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JUtility/JUTFont.h"
#include <cstdarg>
#include <cstdio>

class JUTConsole;

// These are declared extern "C" in the original so that C code can call them.
extern "C" void JUTConsole_print_f_va_(JUTConsole*, const char*, va_list);
extern "C" void JUTSetReportConsole(JUTConsole*);
extern "C" JUTConsole* JUTGetReportConsole();
extern "C" void JUTSetWarningConsole(JUTConsole*);
extern "C" JUTConsole* JUTGetWarningConsole();
extern "C" void JUTWarningConsole_f_va(const char*, va_list);
extern "C" void JUTReportConsole_f_va(const char*, va_list);
extern "C" void JUTReportConsole_f(const char*, ...);
extern "C" void JUTWarningConsole(const char* message);
extern "C" void JUTWarningConsole_f(const char* message, ...);
extern "C" void JUTReportConsole(const char* message);

class JUTConsole : public JKRDisposer {
public:
    enum EConsoleType {
        CONSOLE_TYPE_0 = 0,
        CONSOLE_TYPE_1 = 1,
        CONSOLE_TYPE_2 = 2,
    };
    enum OutputFlag {
        OUTPUT_NONE            = 0,
        OUTPUT_OSREPORT        = 1,
        OUTPUT_CONSOLE         = 2,
        OUTPUT_OSR_AND_CONSOLE = 3,
    };

    // Factory methods — return a static stub instance
    static JUTConsole* create(unsigned int lines, void* /*buf*/, u32 /*bufSize*/) {
        return getStub(lines);
    }
    static JUTConsole* create(unsigned int lines, unsigned int /*linelen*/, JKRHeap* /*heap*/) {
        return getStub(lines);
    }

    JUTConsole(unsigned int /*linelen*/, unsigned int /*maxlines*/, bool /*own*/)
        : mMaxLines(64), mBuf(nullptr), mPositionX(0), mPositionY(0)
        , mHeight(64), mFont(nullptr), mFontSizeX(8.0f), mFontSizeY(16.0f)
        , mOutput(OUTPUT_OSR_AND_CONSOLE), mVisible(true)
        , field_0x20(0), field_0x2c(false), field_0x30(0), field_0x34(0)
        , field_0x38(0), field_0x3c(0), field_0x64(0)
        , field_0x69(false), field_0x6a(false), field_0x6b(false)
    {}

    virtual ~JUTConsole() {}

    static size_t getObjectSizeFromBufferSize(unsigned int, unsigned int) { return sizeof(JUTConsole); }
    static size_t getLineFromObjectSize(u32, unsigned int) { return 0; }

    void clear()   {}
    void doDraw(EConsoleType /*type*/) const {}   // no-op on PC

    void print(char const* str) {
        if (str) ::printf("%s", str);
    }
    void print_f(char const* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ::vprintf(fmt, args);
        va_end(args);
    }
    void print_f_va(const char* fmt, va_list args) {
        JUTConsole_print_f_va_(this, fmt, args);
    }
    void dumpToTerminal(unsigned int /*start*/) {}
    void scroll(int /*lines*/)    {}
    int  getUsedLine()   const { return 0; }
    int  getLineOffset() const { return 0; }

    void setOutput(unsigned int output) { mOutput = output; }
    void setPosition(int x, int y) { mPositionX = x; mPositionY = y; }
    void setFontSize(f32 x, f32 y) { mFontSizeX = x; mFontSizeY = y; }
    void setHeight(unsigned int height) {
        mHeight = height;
        if (mHeight > (unsigned)mMaxLines) mHeight = (unsigned)mMaxLines;
    }
    void setFont(JUTFont* p_font) {
        mFont = p_font;
        if (p_font) setFontSize((f32)p_font->getWidth(), (f32)p_font->getHeight());
    }
    void setVisible(bool visible) { mVisible = visible; }

    u32 getOutput()    const { return (u32)mOutput; }
    int getPositionX() const { return mPositionX; }
    int getPositionY() const { return mPositionY; }
    u32 getHeight()    const { return mHeight; }
    bool isVisible()   const { return mVisible; }

    u8  getLineAttr(int /*i*/)           const { return 0; }
    void setLineAttr(int /*i*/, u8 /*v*/)      {}
    u8* getLinePtr(int /*i*/)            const { return nullptr; }

    int diffIndex(int a, int b) const {
        int diff = b - a;
        return diff >= 0 ? diff : diff + mMaxLines;
    }
    int prevIndex(int index) const { return --index < 0 ? mMaxLines - 1 : index; }
    int nextIndex(int index) const { return (++index >= mMaxLines) ? 0 : index; }

    void scrollToLastLine()  { scroll(mMaxLines); }
    void scrollToFirstLine() { scroll(-mMaxLines); }

    // Members (same offsets as original for binary-compat where needed)
    JGadget::TLinkListNode mListNode;
    unsigned int           field_0x20;
    int                    mMaxLines;
    u8*                    mBuf;
    bool                   field_0x2c;
    int                    field_0x30;
    int                    field_0x34;
    int                    field_0x38;
    int                    field_0x3c;
    int                    mPositionX;
    int                    mPositionY;
    u32                    mHeight;
    JUTFont*               mFont;
    f32                    mFontSizeX;
    f32                    mFontSizeY;
    int                    mOutput;
    JUtility::TColor       field_0x5c;
    JUtility::TColor       field_0x60;
    int                    field_0x64;
    bool                   mVisible;
    bool                   field_0x69;
    bool                   field_0x6a;
    bool                   field_0x6b;

private:
    static JUTConsole* getStub(unsigned int maxLines) {
        static JUTConsole s_stub(64, maxLines, false);
        return &s_stub;
    }
};

class JUTConsoleManager {
public:
    JUTConsoleManager() : mActiveConsole(nullptr), mDirectConsole(nullptr) {}

    static JUTConsoleManager* createManager(JKRHeap* /*heap*/) {
        if (!sManager) sManager = new JUTConsoleManager();
        return sManager;
    }
    void appendConsole(JUTConsole* /*c*/)    {}
    void removeConsole(JUTConsole* /*c*/)    {}
    void draw()                        const {}
    void drawDirect(bool /*force*/)    const {}
    void setDirectConsole(JUTConsole* c) { mDirectConsole = c; }

    JUTConsole* getDirectConsole() const { return mDirectConsole; }

    static JUTConsoleManager* const getManager() { return sManager; }
    static JUTConsoleManager* sManager;

    typedef JGadget::TLinkList<JUTConsole, -static_cast<int>(sizeof(JKRDisposer))> ConsoleList;

private:
    ConsoleList  soLink_;
    JUTConsole*  mActiveConsole;
    JUTConsole*  mDirectConsole;
};
