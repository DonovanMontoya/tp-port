/**
 * src/main.cpp
 * PC port entry point for The Legend of Zelda: Twilight Princess.
 *
 * Boot sequence mirrors the original GC boot:
 *   1. Initialise port layer (window, OS, DVD, PAD, AX/audio)
 *   2. Call the game's own initialisation (mDoMain::boot)
 *   3. Run the game loop
 *   4. Tear down
 *
 * The game's main loop lives in f_ap/f_ap_game.cpp (fapGm_Execute).
 * Uncomment the forward declarations and calls below as those files are
 * added to cmake/game_sources.cmake and confirmed to compile.
 */

#include "port/port.h"
#include "window/window.h"
#include "dolphin/os.h"
#include "dolphin/dvd.h"
#include "dolphin/pad.h"
#include "dolphin/ax.h"
#include "dolphin/gx.h"
#include "m_Do/m_Do_Reset.h"

#include <cstdio>
#include <cstdlib>

// -----------------------------------------------------------------------
// Forward declarations — uncomment as the game source compiles in
// -----------------------------------------------------------------------
void main01(void);        // fallback direct boot path
// extern "C" void fapGm_Execute(void); // f_ap/f_ap_game.cpp

// -----------------------------------------------------------------------
// Port-side game loop stub (runs until window closes)
// -----------------------------------------------------------------------
static void RunGameLoop() {
    while (tp::window::IsOpen()) {
        tp::window::BeginFrame();

        // TODO: call fapGm_Execute() here once game source compiles
        // fapGm_Execute();

        tp::window::EndFrame();
    }
}

// -----------------------------------------------------------------------
// Entry point
// -----------------------------------------------------------------------
#ifdef _WIN32
#define NOMINMAX          // prevent Windows.h from defining min/max macros
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static LONG WINAPI LogUnhandledException(EXCEPTION_POINTERS* ep) {
    if (!ep || !ep->ExceptionRecord) {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    const auto* rec = ep->ExceptionRecord;
    HMODULE module = GetModuleHandleA(nullptr);
    auto base = reinterpret_cast<uintptr_t>(module);
    auto addr = reinterpret_cast<uintptr_t>(rec->ExceptionAddress);
    tp::log::error("Unhandled exception: code=0x%08X address=%p module_base=%p rva=0x%llX",
                   static_cast<unsigned>(rec->ExceptionCode), rec->ExceptionAddress, module,
                   static_cast<unsigned long long>(addr - base));

    void* frames[16] = {};
    USHORT count = CaptureStackBackTrace(0, 16, frames, nullptr);
    for (USHORT i = 0; i < count; ++i) {
        auto frame = reinterpret_cast<uintptr_t>(frames[i]);
        if (frame >= base) {
            tp::log::error("  stack[%u] = %p (rva=0x%llX)", static_cast<unsigned>(i), frames[i],
                           static_cast<unsigned long long>(frame - base));
        } else {
            tp::log::error("  stack[%u] = %p", static_cast<unsigned>(i), frames[i]);
        }
    }

    return EXCEPTION_CONTINUE_SEARCH;
}
#endif

static void InitResetStateIfNeeded() {
    if (mDoRst::getResetData()) {
        return;
    }

    mDoRst::setResetData(static_cast<mDoRstData*>(OSAllocFromArenaLo(0x18, 4)));
    if (!mDoRst::getResetData()) {
        return;
    }

    if ((OSGetResetCode() & 0x80000000u) == 0) {
        mDoRst::offReset();
        mDoRst::offResetPrepare();
        mDoRst::off3ButtonReset();
        mDoRst::set3ButtonResetPort(-1);
        mDoRst::setLogoScnFlag(0);
        mDoRst::setProgSeqFlag(0);
        mDoRst::setProgChgFlag(0);
        mDoRst::setWarningDispFlag(0);
        mDoRst::offShutdown();
        mDoRst::offReturnToMenu();
    }
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrev*/,
                   LPSTR /*cmdLine*/, int /*showCmd*/)
#else
int main(int /*argc*/, char** /*argv*/)
#endif
{
#ifdef _WIN32
    SetUnhandledExceptionFilter(LogUnhandledException);
#endif
    // Parse TP_DATA_PATH from environment (also accepted in DVDInit)
    const char* dataPath = getenv("TP_DATA_PATH");
    if (dataPath)
        tp::log::info("Using game data path: %s", dataPath);
    else
        tp::log::info("TP_DATA_PATH not set — looking for ./gamedata/ or ./port/gamedata/");

    // ── Port layer init ────────────────────────────────────────────────
    OSInit();
    DVDInit();
    InitResetStateIfNeeded();
    if (!DVDHasGameData()) {
        const char* resolvedPath = DVDGetGameDataPath();
        tp::log::error("Game data check failed. Aborting before boot.");
#ifdef _WIN32
        char msg[1024];
        std::snprintf(
            msg, sizeof(msg),
            "Twilight Princess game data was not found.\n\n"
            "Expected extracted data with both 'files' and 'sys' in:\n%s\n\n"
            "Run port/tools/extract_iso.py on your own disc/ISO,\n"
            "or set TP_DATA_PATH to your extracted game data folder.",
            resolvedPath ? resolvedPath : "(unknown)");
        MessageBoxA(nullptr, msg, "Missing Game Data", MB_OK | MB_ICONERROR);
#endif
        DVDQuit();
        return 1;
    }

    tp::window::Config winCfg;
    winCfg.width  = 854;   // widescreen 16:9
    winCfg.height = 480;
    tp::log::info("About to initialize window");
    if (!tp::window::Init(winCfg)) {
        tp::log::error("Window init failed");
        DVDQuit();
        return 1;
    }
    tp::log::info("Window initialized");

    // GXInit must be called after GL context creation
    tp::log::info("About to initialize GX");
    GXInit(nullptr, 0);
    tp::log::info("GX initialized");

    if (!PADInit()) {
        tp::log::warn("PADInit failed — keyboard-only mode");
    }

    tp::log::info("About to initialize AX");
    AXInit();
    tp::log::info("AX initialized");

    // ── Game init + loop ──────────────────────────────────────────────
    // main01() contains the full GC game boot sequence and the infinite
    // do { fapGm_Execute(); } while(true) game loop.
    tp::log::info("Entering main01 (game boot + loop)");
    main01();

    // ── Teardown ──────────────────────────────────────────────────────
    AXQuit();
    tp::window::Shutdown();
    DVDQuit();

    tp::log::info("Clean exit.");
    return 0;
}
