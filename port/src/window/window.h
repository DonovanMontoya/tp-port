#pragma once
/**
 * window/window.h
 * GLFW window + OpenGL context management for the TP PC port.
 */
#include "port/types.h"

namespace tp::window {

struct Config {
    int         width       = 640;
    int         height      = 480;
    const char* title       = "The Legend of Zelda: Twilight Princess";
    bool        fullscreen  = false;
    bool        vsync       = true;
};

bool Init(const Config& cfg);
void Shutdown(void);

// Poll events and swap buffers — called once per frame by the game loop
void BeginFrame(void);
void EndFrame(void);

// Returns true while the window is open
bool IsOpen(void);

// Actual drawable size (may differ from Config on HiDPI displays)
void GetFramebufferSize(int* w, int* h);

// Request the window to close (e.g. on Alt+F4 or Escape)
void RequestClose(void);

// Called by GXCopyDisp equivalent to blit the rendered frame
void Present(void);

} // namespace tp::window
