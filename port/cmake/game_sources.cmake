# -----------------------------------------------------------------------
# Game source list
#
# Add files here incrementally as they are verified to compile cleanly
# under a standard C++17 compiler with the port abstraction layer.
#
# Workflow:
#   1. Add a source file to GAME_SOURCES below.
#   2. Build and fix compile errors (usually missing stubs or GC types).
#   3. Repeat until the full game compiles.
# -----------------------------------------------------------------------

set(DECOMP_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../src")
set(DECOMP_INC "${CMAKE_CURRENT_SOURCE_DIR}/../include")

set(GAME_SOURCES

  # ── SSystem/SComponent — utility/math (minimal dependencies) ─────────
  ${DECOMP_SRC}/SSystem/SComponent/c_counter.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_request.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_phase.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_node.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_node_iter.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_list.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_list_iter.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_tree.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_tree_iter.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_tag.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_tag_iter.cpp

  # ── SSystem/SComponent — 3-D math geometry primitives ─────────────────
  ${DECOMP_SRC}/SSystem/SComponent/c_m3d_g_vtx.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_m3d_g_cir.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_m3d_g_cps.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_m3d_g_lin.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_m3d_g_pla.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_m3d_g_sph.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_m3d_g_tri.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_m3d_g_cyl.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_m3d_g_aab.cpp

  # ── SSystem/SComponent — math & angles ───────────────────────────────
  ${DECOMP_SRC}/SSystem/SComponent/c_math.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_angle.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_sxyz.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_xyz.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_m2d.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_m3d.cpp

  # ── SSystem/SComponent — background/collision helpers ─────────────────
  ${DECOMP_SRC}/SSystem/SComponent/c_bg_w.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_bg_s_chk.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_bg_s_lin_chk.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_bg_s_gnd_chk.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_bg_s_poly_info.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_bg_s_shdw_draw.cpp

  # ── SSystem/SComponent — common library & API wrappers ────────────────
  ${DECOMP_SRC}/SSystem/SComponent/c_lib.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_API.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_API_graphic.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_API_controller_pad.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_cc_s.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_cc_d.cpp
  ${DECOMP_SRC}/SSystem/SComponent/c_malloc.cpp

  # ── SSystem/SStandard ─────────────────────────────────────────────────
  ${DECOMP_SRC}/SSystem/SStandard/s_basic.cpp

  # ── Core framework ────────────────────────────────────────────────────
  # These pull in more dependencies; uncomment as porting progresses.
  # ${DECOMP_SRC}/f_op/f_op_actor.cpp
  # ${DECOMP_SRC}/f_op/f_op_actor_mng.cpp
  # ${DECOMP_SRC}/f_op/f_op_scene.cpp
  # ${DECOMP_SRC}/f_op/f_op_scene_mng.cpp
  # ${DECOMP_SRC}/f_op/f_op_kankyo.cpp
  # ${DECOMP_SRC}/f_op/f_op_overlap.cpp
  # ${DECOMP_SRC}/f_op/f_op_camera.cpp
  # ${DECOMP_SRC}/f_op/f_op_draw_tag.cpp

  # ── Main module ───────────────────────────────────────────────────────
  # ${DECOMP_SRC}/m_Do/m_Do_main.cpp
  # ${DECOMP_SRC}/m_Do/m_Do_graphic.cpp
  # ${DECOMP_SRC}/m_Do/m_Do_audio.cpp
  # ${DECOMP_SRC}/m_Do/m_Do_controller_pad.cpp
  # ${DECOMP_SRC}/m_Do/m_Do_dvd_thread.cpp
  # ${DECOMP_SRC}/m_Do/m_Do_heap.cpp
  # ${DECOMP_SRC}/m_Do/m_Do_memCardRW.cpp
)

# Always compile the port bootstrap
list(APPEND GAME_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)
