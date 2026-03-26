#pragma once
/**
 * port/include/JSystem/JUtility/JUTException.h
 * PC port stub — GC exception / crash handler.
 * All methods are no-ops; no GX framebuffer, no OS exception vectors on PC.
 */
#include "JSystem/JKernel/JKRThread.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "JSystem/JSupport/JSUList.h"
#include "port/types.h"
#include <cstdarg>

// Forward declare GC types we can't use on PC
class JUTConsole;
class JUTDirectPrint;
struct OSContext;
typedef unsigned short OSError;
typedef void (*JUTExceptionUserCallback)(u16, OSContext*, u32, u32);

// GXGamma and GXRenderModeObj are defined in dolphin/gx/GXEnum.h and
// GXStruct.h (pulled in via dolphin/gx.h).  Only define them here as
// fallback stubs when those GC headers haven't been included.
#ifndef _DOLPHIN_GX_GXENUM_H_
typedef int GXGamma;
#endif

// ---------------------------------------------------------------------------
// JUTExternalFB — framebuffer stub
// ---------------------------------------------------------------------------
// GXRenderModeObj is defined in dolphin/gx/GXStruct.h when GC headers are
// present.  Avoid a conflicting 'struct' forward-declaration in that case.
#ifndef _DOLPHIN_GX_GXSTRUCT_H_
struct GXRenderModeObj;
#endif

class JUTExternalFB {
public:
    JUTExternalFB(GXRenderModeObj*, GXGamma, void*, u32) {}
};

// ---------------------------------------------------------------------------
// Preprocessor constants from original
// ---------------------------------------------------------------------------
#define JUT_PRINT_GPR       1
#define JUT_PRINT_GPR_MAP   2
#define JUT_PRINT_SRR0_MAP  4
#define JUT_PRINT_FLOAT     8
#define JUT_PRINT_STACK     16

// ---------------------------------------------------------------------------
// JUTException stub
// ---------------------------------------------------------------------------
class JUTException : public JKRThread {
public:
    enum EInfoPage {
        EINFO_PAGE_GPR     = 1,
        EINFO_PAGE_FLOAT   = 2,
        EINFO_PAGE_STACK   = 3,
        EINFO_PAGE_GPR_MAP = 4,
        EINFO_PAGE_SSR0_MAP = 5,
    };

    class JUTExMapFile {
    public:
        JUTExMapFile(const char* path) : mLink(this) { mPath = const_cast<char*>(path); }
        char*                  mPath;
        JSULink<JUTExMapFile>  mLink;
    };

    JUTException(JUTDirectPrint*) : JKRThread(static_cast<OSThread*>(nullptr), 0) {}
    virtual ~JUTException() {}

    void showFloatSub(int, f32)                          {}
    void showFloat(OSContext*)                           {}
    void showStack(OSContext*)                           {}
    void showMainInfo(u16, OSContext*, u32, u32)         {}
    void showGPR(OSContext*)                             {}
    bool showMapInfo_subroutine(u32, bool)               { return false; }
    void showGPRMap(OSContext*)                          {}
    void showSRR0Map(OSContext*)                         {}
    void printDebugInfo(EInfoPage, OSError, OSContext*, u32, u32) {}
    bool isEnablePad()                             const { return false; }
    bool readPad(u32*, u32*)                             { return false; }
    void printContext(u16, OSContext*, u32, u32)         {}
    void createFB()                                      {}

    virtual void* run() override { return nullptr; }

    static JUTException*          create(JUTDirectPrint*)             { return nullptr; }
    static void                   errorHandler(OSError, OSContext*, u32, u32) {}
    static void                   panic_f_va(char const*, int, char const*, va_list) {}
    static void                   panic_f(char const* file, int line, char const* fmt, ...) {}
    static void                   setFPException(u32)                 {}
    static bool                   searchPartialModule(u32, u32*, u32*, u32*, u32*) { return false; }
    static JUTExceptionUserCallback setPreUserCallback(JUTExceptionUserCallback cb)  { return nullptr; }
    static JUTExceptionUserCallback setPostUserCallback(JUTExceptionUserCallback cb) { return nullptr; }
    static void                   appendMapFile(char const*)          {}
    static bool                   queryMapAddress(char*, u32, s32, u32*, u32*, char*, u32, bool, bool) { return false; }
    static bool                   queryMapAddress_single(char*, u32, s32, u32*, u32*, char*, u32, bool, bool) { return false; }
    static void                   createConsole(void*, u32)           {}
    static void                   waitTime(s32)                       {}

    static JUTException* getManager() { return sErrorManager; }
    static JUTConsole*   getConsole()  { return sConsole; }

    JUTExternalFB* getFrameMemory() const { return nullptr; }
    void setTraceSuppress(u32)      {}
    void setGamePad(JUTGamePad*)    {}

    static void setMapFile(const char* map) { appendMapFile(map); }
    static void panic(const char* file, int line, const char* msg) {
        panic_f(file, line, "%s", msg);
    }

private:
    static JUTException*             sErrorManager;
    static JUTExceptionUserCallback  sPreUserCallback;
    static JUTExceptionUserCallback  sPostUserCallback;
    static void*                     sConsoleBuffer;
    static u32                       sConsoleBufferSize;
    static JUTConsole*               sConsole;
};

// ---------------------------------------------------------------------------
// JUTWarn — stream-style warning sink
// ---------------------------------------------------------------------------
struct JUTWarn {
    JUTWarn& operator<<(const char*) { return *this; }
    JUTWarn& operator<<(s32)         { return *this; }
};
