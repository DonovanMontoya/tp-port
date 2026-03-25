/**
 * window/window.cpp
 * GLFW + OpenGL 4.5 window management.
 */

#include "window/window.h"
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
    glfwSetErrorCallback(GLFWErrorCallback);
    if (!glfwInit()) {
        tp::log::error("glfwInit failed");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWmonitor* monitor = cfg.fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    sWindow = glfwCreateWindow(cfg.width, cfg.height, cfg.title, monitor, nullptr);
    if (!sWindow) {
        tp::log::error("glfwCreateWindow failed");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(sWindow);
    glfwSwapInterval(cfg.vsync ? 1 : 0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        tp::log::error("gladLoadGLLoader failed");
        glfwTerminate();
        return false;
    }

    // Enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLDebugCallback, nullptr);

    // Key callback: Escape → close, F11 → toggle fullscreen
    glfwSetKeyCallback(sWindow, [](GLFWwindow* w, int key, int /*sc*/, int action, int /*mod*/) {
        if (action == GLFW_PRESS) {
            if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(w, GLFW_TRUE);
        }
    });

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EndFrame(void) {
    glfwSwapBuffers(sWindow);
}

bool IsOpen(void) {
    return sWindow && !glfwWindowShouldClose(sWindow);
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
