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
  # Game version — GZ2E01 (GameCube North America) = VERSION_GCN_USA = 0
  GZ2E01=1
  VERSION=0
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
  # Also add the dolphin/ subdirectory so bare #include <types.h> etc. work
  "${DECOMP_ROOT}/libs/dolphin/include/dolphin"
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

# Helper to apply the port.h force-include only to our CXX targets, NOT
# to third-party C libraries (GLFW etc.) which cannot include C++ headers.
if(MSVC)
  set(PORT_FORCE_INCLUDE "/FI${CMAKE_CURRENT_SOURCE_DIR}/include/port/port.h")
else()
  set(PORT_FORCE_INCLUDE "-include${CMAKE_CURRENT_SOURCE_DIR}/include/port/port.h")
endif()

add_library(tp_port_impl STATIC ${PORT_SOURCES})
target_include_directories(tp_port_impl PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_compile_options(tp_port_impl PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${PORT_FORCE_INCLUDE}>)
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

# m_Do_main.cpp has `void main(int,char**)` (GC CodeWarrior style — not
# standard C++).  Rename it to `gc_main` so GCC doesn't complain.
set_source_files_properties(
  "${DECOMP_ROOT}/src/m_Do/m_Do_main.cpp"
  PROPERTIES COMPILE_FLAGS "-Dmain=gc_main"
)

add_executable(TwilightPrincess WIN32
  ${GAME_SOURCES}
)

target_compile_options(TwilightPrincess PRIVATE
  $<$<COMPILE_LANGUAGE:CXX>:${PORT_FORCE_INCLUDE}>
  # GC toolchain uses `void main()` which is non-standard; -fpermissive
  # downgrades that hard error to a warning so the file still compiles.
  # These flags are GCC/Clang-only — MSVC uses -Dmain=gc_main instead.
  $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fpermissive>
)
target_include_directories(TwilightPrincess PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${DECOMP_ROOT}/src"
  "${DECOMP_ROOT}/include"
  "${DECOMP_ROOT}/libs/JSystem/include"
  "${DECOMP_ROOT}/libs/dolphin/include"
  "${DECOMP_ROOT}/libs/dolphin/include/dolphin"
  "${DECOMP_ROOT}/libs/revolution/include"
  "${DECOMP_ROOT}/libs/PowerPC_EABI_Support/include"
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

# -----------------------------------------------------------------------
# Copy runtime DLLs next to the executable on Windows
# -----------------------------------------------------------------------
if(WIN32)
  add_custom_command(TARGET TwilightPrincess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:OpenAL::OpenAL>
      $<TARGET_FILE_DIR:TwilightPrincess>
    COMMENT "Copying OpenAL32.dll to output directory"
  )
endif()
