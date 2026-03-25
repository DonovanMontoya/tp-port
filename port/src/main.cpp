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

#include <cstdio>
#include <cstdlib>

// -----------------------------------------------------------------------
// Forward declarations — uncomment as the game source compiles in
// -----------------------------------------------------------------------
extern "C" void main01(void);        // m_Do/m_Do_main.cpp — the real game boot + loop
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
#include <windows.h>
int WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrev*/,
                   LPSTR /*cmdLine*/, int /*showCmd*/)
#else
int main(int /*argc*/, char** /*argv*/)
#endif
{
    // Parse TP_DATA_PATH from environment (also accepted in DVDInit)
    const char* dataPath = getenv("TP_DATA_PATH");
    if (dataPath)
        tp::log::info("Using game data path: %s", dataPath);
    else
        tp::log::info("TP_DATA_PATH not set — looking for ./gamedata/");

    // ── Port layer init ────────────────────────────────────────────────
    OSInit();
    DVDInit();

    tp::window::Config winCfg;
    winCfg.width  = 854;   // widescreen 16:9
    winCfg.height = 480;
    if (!tp::window::Init(winCfg)) {
        tp::log::error("Window init failed");
        DVDQuit();
        return 1;
    }

    // GXInit must be called after GL context creation
    GXInit(nullptr, 0);

    if (!PADInit()) {
        tp::log::warn("PADInit failed — keyboard-only mode");
    }

    AXInit();

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
