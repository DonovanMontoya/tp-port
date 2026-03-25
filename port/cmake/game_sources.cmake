# -----------------------------------------------------------------------
# Game source list
#
# Add files here incrementally as they are verified to compile cleanly
# under a standard C++17 compiler with the port abstraction layer.
#
# The decomp produces ~1,085 .cpp files.  Not all will compile on the
# first pass — hardware-specific code, inline assembly, and files that
# rely on Metrowerks extensions need porting work first.
#
# Workflow:
#   1. Add a source file to GAME_SOURCES below.
#   2. Build and fix compile errors (usually missing stubs or GC types).
#   3. Repeat until the full game compiles.
# -----------------------------------------------------------------------

set(DECOMP_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../src")

set(GAME_SOURCES
  # ── Core framework ────────────────────────────────────────────────
  # ${DECOMP_SRC}/f_op/f_op_actor.cpp
  # ${DECOMP_SRC}/f_op/f_op_actor_mng.cpp
  # ${DECOMP_SRC}/f_op/f_op_scene.cpp
  # ${DECOMP_SRC}/f_op/f_op_scene_mng.cpp

  # ── Main module ───────────────────────────────────────────────────
  # ${DECOMP_SRC}/m_Do/m_Do_main.cpp
  # ${DECOMP_SRC}/m_Do/m_Do_graphic.cpp
  # ${DECOMP_SRC}/m_Do/m_Do_audio.cpp
  # ${DECOMP_SRC}/m_Do/m_Do_controller_pad.cpp
  # ${DECOMP_SRC}/m_Do/m_Do_dvd_thread.cpp

  # ── Add more files as porting progresses ──────────────────────────
)

# Always compile the port bootstrap
list(APPEND GAME_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)
