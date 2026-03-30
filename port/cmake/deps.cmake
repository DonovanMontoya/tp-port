# -----------------------------------------------------------------------
# Third-party dependency management via CMake FetchContent
# All deps are downloaded automatically on first configure.
# -----------------------------------------------------------------------
include(FetchContent)

function(tp_port_declare_local_or_git name local_source git_repository git_tag)
  if(EXISTS "${local_source}/CMakeLists.txt")
    message(STATUS "Using local ${name} checkout: ${local_source}")
    FetchContent_Declare(${name}
      SOURCE_DIR "${local_source}"
    )
  else()
    FetchContent_Declare(${name}
      GIT_REPOSITORY ${git_repository}
      GIT_TAG        ${git_tag}
      GIT_SHALLOW    TRUE
    )
  endif()
endfunction()

# CMake 4 removed compatibility with projects that still declare
# cmake_minimum_required(VERSION 3.0). glad v0.1.36 still does that, so
# lower the policy floor for fetched subprojects to keep configure working.
if(CMAKE_VERSION VERSION_GREATER_EQUAL 4.0)
  set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
endif()

# GLFW — windowing & OpenGL context
tp_port_declare_local_or_git(
  glfw
  "${CMAKE_CURRENT_SOURCE_DIR}/build-make/_deps/glfw-src"
  https://github.com/glfw/glfw.git
  3.4
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# glad — OpenGL 4.5 core loader
tp_port_declare_local_or_git(
  glad
  "${CMAKE_CURRENT_SOURCE_DIR}/build-make/_deps/glad-src"
  https://github.com/Dav1dde/glad.git
  v0.1.36
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
set(TP_PORT_LOCAL_OPENAL_SOURCE
  "${CMAKE_CURRENT_SOURCE_DIR}/build-make/_deps/openal-src"
)
if(NOT EXISTS "${TP_PORT_LOCAL_OPENAL_SOURCE}/CMakeLists.txt")
  set(TP_PORT_LOCAL_OPENAL_SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/../.claude/worktrees/determined-lamport/port/build/_deps/openal-src"
  )
endif()
tp_port_declare_local_or_git(
  openal
  "${TP_PORT_LOCAL_OPENAL_SOURCE}"
  https://github.com/kcat/openal-soft.git
  1.23.1
)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_TESTS    OFF CACHE BOOL "" FORCE)
set(ALSOFT_UTILS    OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(openal)

# SDL2 — controller / input (PAD replacement)
tp_port_declare_local_or_git(
  sdl2
  "${CMAKE_CURRENT_SOURCE_DIR}/build-make/_deps/sdl2-src"
  https://github.com/libsdl-org/SDL.git
  release-2.30.3
)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON  CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(sdl2)

# GLM — math library (mtx / vec helpers)
tp_port_declare_local_or_git(
  glm
  "${CMAKE_CURRENT_SOURCE_DIR}/build-make/_deps/glm-src"
  https://github.com/g-truc/glm.git
  1.0.1
)
FetchContent_MakeAvailable(glm)
