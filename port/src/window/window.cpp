/**
 * window/window.cpp
 * GLFW + OpenGL 4.5 window management.
 */

#include "window.h"
#include "port/port.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>

static GLFWwindow* sWindow = nullptr;

static void GLFWErrorCallback(int code, const char* desc) {
    tp::log::error("GLFW error %d: %s", code, desc);
}

static void GLAPIENTRY GLDebugCallback(GLenum /*src*/, GLenum type, GLuint /*id*/,
                                        GLenum severity, GLsizei /*len*/,
                                        const GLchar* msg, const void* /*param*/)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
    tp::log::warn("GL [%s]: %s",
                  type == GL_DEBUG_TYPE_ERROR ? "ERROR" : "WARN", msg);
}

namespace tp::window {

bool Init(const Config& cfg) {
    tp::log::info("window::Init begin");
    glfwSetErrorCallback(GLFWErrorCallback);
#ifdef __APPLE__
    glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
    glfwInitHint(GLFW_COCOA_MENUBAR, GLFW_FALSE);
#endif
    tp::log::info("Calling glfwInit");
    if (!glfwInit()) {
        tp::log::error("glfwInit failed");
        return false;
    }
    tp::log::info("glfwInit succeeded");

    int glMajor = 4;
    int glMinor = 5;
#ifdef __APPLE__
    glMinor = 1;
#endif
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    tp::log::info("Requesting OpenGL context %d.%d", glMajor, glMinor);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWmonitor* monitor = cfg.fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    tp::log::info("Calling glfwCreateWindow");
    sWindow = glfwCreateWindow(cfg.width, cfg.height, cfg.title, monitor, nullptr);
    if (!sWindow) {
        tp::log::error("glfwCreateWindow failed");
        glfwTerminate();
        return false;
    }
    tp::log::info("glfwCreateWindow succeeded");

    tp::log::info("Making GL context current");
    glfwMakeContextCurrent(sWindow);
    glfwSwapInterval(cfg.vsync ? 1 : 0);

    tp::log::info("Loading GL via glad");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        tp::log::error("gladLoadGLLoader failed");
        glfwTerminate();
        return false;
    }
    tp::log::info("gladLoadGLLoader succeeded");

    tp::log::info("Configuring GL debug output");
#if defined(__APPLE__)
    tp::log::info("Skipping GL debug callback on macOS");
#else
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLDebugCallback, nullptr);
#endif

    // Key callback: Escape → close, F11 → toggle fullscreen
    tp::log::info("Installing key callback");
    glfwSetKeyCallback(sWindow, [](GLFWwindow* w, int key, int /*sc*/, int action, int /*mod*/) {
        if (action == GLFW_PRESS) {
            if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(w, GLFW_TRUE);
        }
    });

    tp::log::info("Querying GL version string");
    tp::log::info("Window: %dx%d, GL %s", cfg.width, cfg.height, glGetString(GL_VERSION));
    return true;
}

void Shutdown(void) {
    if (sWindow) { glfwDestroyWindow(sWindow); sWindow = nullptr; }
    glfwTerminate();
}

void BeginFrame(void) {
    glfwPollEvents();
    int w, h;
    glfwGetFramebufferSize(sWindow, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);  // dark blue — visible placeholder
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EndFrame(void) {
    glfwSwapBuffers(sWindow);
}

bool IsOpen(void) {
    static bool s_logged_close = false;
    bool open = sWindow && !glfwWindowShouldClose(sWindow);
    if (!open && !s_logged_close) {
        s_logged_close = true;
        tp::log::warn("window::IsOpen -> false (sWindow=%p shouldClose=%d)",
                      sWindow, sWindow ? glfwWindowShouldClose(sWindow) : -1);
    }
    return open;
}

void GetFramebufferSize(int* w, int* h) {
    if (sWindow) glfwGetFramebufferSize(sWindow, w, h);
    else { if (w) *w = 640; if (h) *h = 480; }
}

void RequestClose(void) {
    if (sWindow) glfwSetWindowShouldClose(sWindow, GLFW_TRUE);
}

void Present(void) {
    glfwSwapBuffers(sWindow);
}

} // namespace tp::window
