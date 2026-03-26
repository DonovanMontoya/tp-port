# -----------------------------------------------------------------------
# Third-party dependency management via CMake FetchContent
# All deps are downloaded automatically on first configure.
# -----------------------------------------------------------------------
include(FetchContent)

# CMake 4 removed compatibility with projects that still declare
# cmake_minimum_required(VERSION 3.0). glad v0.1.36 still does that, so
# lower the policy floor for fetched subprojects to keep configure working.
if(CMAKE_VERSION VERSION_GREATER_EQUAL 4.0)
  set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
endif()

# GLFW — windowing & OpenGL context
FetchContent_Declare(glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4
  GIT_SHALLOW    TRUE
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# glad — OpenGL 4.5 core loader
FetchContent_Declare(glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG        v0.1.36
  GIT_SHALLOW    TRUE
)
set(GLAD_PROFILE      "core"   CACHE STRING "" FORCE)
set(GLAD_API          "gl=4.5" CACHE STRING "" FORCE)
set(GLAD_GENERATOR    "c"      CACHE STRING "" FORCE)
set(GLAD_EXTENSIONS   ""       CACHE STRING "" FORCE)
set(GLAD_SPEC         "gl"     CACHE STRING "" FORCE)
set(GLAD_REPRODUCIBLE OFF      CACHE BOOL   "" FORCE)
FetchContent_MakeAvailable(glad)

# glad v0.1.36 opens packaged XML specs in text mode; with Python 3.14 that
# can trip over the UTF-8 BOM during generation. Staging the packaged spec in
# the source root makes glad parse it by filename instead of by file object.
if(DEFINED glad_SOURCE_DIR AND EXISTS "${glad_SOURCE_DIR}/glad/files/gl.xml")
  file(COPY "${glad_SOURCE_DIR}/glad/files/gl.xml" DESTINATION "${glad_SOURCE_DIR}")
endif()

# OpenAL Soft — audio (AX replacement)
FetchContent_Declare(openal
  GIT_REPOSITORY https://github.com/kcat/openal-soft.git
  GIT_TAG        1.23.1
  GIT_SHALLOW    TRUE
)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_TESTS    OFF CACHE BOOL "" FORCE)
set(ALSOFT_UTILS    OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(openal)

# SDL2 — controller / input (PAD replacement)
FetchContent_Declare(sdl2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-2.30.3
  GIT_SHALLOW    TRUE
)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON  CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(sdl2)

# GLM — math library (mtx / vec helpers)
FetchContent_Declare(glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        1.0.1
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(glm)
