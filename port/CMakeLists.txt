cmake_minimum_required(VERSION 3.20)
project(TwilightPrincessPC VERSION 0.1.0 LANGUAGES C CXX)

# -----------------------------------------------------------------------
# Standards & global flags
# -----------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# -----------------------------------------------------------------------
# Platform definitions
# -----------------------------------------------------------------------
add_compile_definitions(
  PLATFORM_PC=1
  PLATFORM_GC=0
  PLATFORM_WII=0
  PLATFORM_SHIELD=0
  # Game version — GZ2E01 (GameCube North America) by default
  GZ2E01=1
)

if(WIN32)
  add_compile_definitions(_WIN32_WINNT=0x0601 NOMINMAX WIN32_LEAN_AND_MEAN)
endif()

# -----------------------------------------------------------------------
# Endianness: GameCube is big-endian; we supply runtime helpers instead
# -----------------------------------------------------------------------
include(TestBigEndian)
test_big_endian(HOST_IS_BIG_ENDIAN)
if(NOT HOST_IS_BIG_ENDIAN)
  add_compile_definitions(TP_LITTLE_ENDIAN=1)
else()
  add_compile_definitions(TP_LITTLE_ENDIAN=0)
endif()

# -----------------------------------------------------------------------
# Include paths — port headers shadow the decomp's dolphin headers
# -----------------------------------------------------------------------
set(DECOMP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")

include_directories(
  # Port-specific overrides must come FIRST
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  # Decomp game source
  "${DECOMP_ROOT}/src"
  "${DECOMP_ROOT}/include"
  # Decomp libs (JSystem, etc.) — GC SDK headers are shadowed by port/include
  "${DECOMP_ROOT}/libs/JSystem/include"
  "${DECOMP_ROOT}/libs/dolphin/include"
  "${DECOMP_ROOT}/libs/revolution/include"
  "${DECOMP_ROOT}/libs/PowerPC_EABI_Support/include"
)

# -----------------------------------------------------------------------
# Third-party dependencies (fetched automatically)
# -----------------------------------------------------------------------
include(cmake/deps.cmake)

# -----------------------------------------------------------------------
# Port implementation library
# -----------------------------------------------------------------------
file(GLOB_RECURSE PORT_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_library(tp_port_impl STATIC ${PORT_SOURCES})
target_include_directories(tp_port_impl PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(tp_port_impl PUBLIC
  glfw
  glad
  OpenAL::OpenAL
  SDL2::SDL2
)

# -----------------------------------------------------------------------
# Game source files
# NOTE: As decompilation matures, add source files incrementally here.
#       Start with only what compiles cleanly under a standard C++17 compiler.
# -----------------------------------------------------------------------
include(cmake/game_sources.cmake)

add_executable(TwilightPrincess WIN32
  ${GAME_SOURCES}
)

target_link_libraries(TwilightPrincess PRIVATE
  tp_port_impl
)

if(WIN32)
  target_link_libraries(TwilightPrincess PRIVATE
    winmm
    ole32
    uuid
    xinput
  )
endif()

# Strip symbols in release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  if(MSVC)
    target_link_options(TwilightPrincess PRIVATE /SUBSYSTEM:WINDOWS)
  endif()
endif()
