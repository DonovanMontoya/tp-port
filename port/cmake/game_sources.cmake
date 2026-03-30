# -----------------------------------------------------------------------
# Game source list
#
# ACGC-style experiment:
#   Compile the decomp broadly by default, then maintain a small exclusion
#   list for translation units that still need more platform work.
#
# This flips the old workflow on its head:
#   old: add files manually once they compile
#   new: compile everything and carve out blockers
# -----------------------------------------------------------------------

set(DECOMP_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../src")

file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
  "${DECOMP_SRC}/*.c"
  "${DECOMP_SRC}/*.cpp"
)

file(GLOB TP_PORT_ACTOR_SOURCES CONFIGURE_DEPENDS
  "${DECOMP_SRC}/d/actor/*.cpp"
)

set(TP_PORT_ACTOR_KEEP_SOURCES
  "${DECOMP_SRC}/d/actor/d_a_title.cpp"
)

list(REMOVE_ITEM TP_PORT_ACTOR_SOURCES ${TP_PORT_ACTOR_KEEP_SOURCES})

file(GLOB TP_PORT_TOPLEVEL_ACTOR_SUPPORT_SOURCES CONFIGURE_DEPENDS
  "${DECOMP_SRC}/d/d_a*.cpp"
)

file(GLOB TP_PORT_TOPLEVEL_D_SOURCES CONFIGURE_DEPENDS
  "${DECOMP_SRC}/d/*.cpp"
)

set(TP_PORT_TOPLEVEL_D_KEEP_SOURCES
  "${DECOMP_SRC}/d/d_attention.cpp"
  "${DECOMP_SRC}/d/d_com_inf_game.cpp"
  "${DECOMP_SRC}/d/d_resorce.cpp"
  "${DECOMP_SRC}/d/d_save.cpp"
  "${DECOMP_SRC}/d/d_s_logo.cpp"
  "${DECOMP_SRC}/d/d_s_menu.cpp"
  "${DECOMP_SRC}/d/d_s_play.cpp"
  "${DECOMP_SRC}/d/d_s_title.cpp"
  "${DECOMP_SRC}/d/d_simple_model.cpp"
)

list(REMOVE_ITEM TP_PORT_TOPLEVEL_D_SOURCES ${TP_PORT_TOPLEVEL_D_KEEP_SOURCES})

set(TP_PORT_EXCLUDED_GAME_SOURCES
  # Actor code is the biggest gameplay dependency fan-out in the project.
  # Exclude the whole slice for now so the fast-port branch can reach a
  # runnable title/menu executable before backfilling gameplay actors.
  ${TP_PORT_ACTOR_SOURCES}
  ${TP_PORT_TOPLEVEL_ACTOR_SUPPORT_SOURCES}
  ${TP_PORT_TOPLEVEL_D_SOURCES}

  # REL loader path still assumes a GC-specific archive / heap / 32-bit pointer
  # environment. Keep it out until the PC module-loading model is redesigned.
  "${DECOMP_SRC}/DynamicLink.cpp"
  "${DECOMP_SRC}/c/c_dylink.cpp"

  # f_pc_profile_lst references nearly every scene and actor profile.
  # Keep using the port-side stub until the full profile table is viable.
  "${DECOMP_SRC}/f_pc/f_pc_profile_lst.cpp"

  # Particle callback integration is still missing pieces of the JPA/JGeometry
  # shim surface. Defer it until the particle subsystem gets a focused pass.
  "${DECOMP_SRC}/c/c_damagereaction.cpp"

  # These units still rely on GC-specific heap/card paths that are not wired
  # up cleanly in the port layer yet.
  "${DECOMP_SRC}/m_Do/m_Do_heap.cpp"
  "${DECOMP_SRC}/m_Do/m_Do_memCardRW.cpp"
  "${DECOMP_SRC}/m_Re/m_Re_controller_pad.cpp"

  # The Z2AudioCS speaker pipeline still mixes Revolution GX/OS headers into
  # host builds and needs a dedicated porting pass instead of ad hoc fixes.
  "${DECOMP_SRC}/Z2AudioCS/SpkData.cpp"
  "${DECOMP_SRC}/Z2AudioCS/SpkMixingBuffer.cpp"
  "${DECOMP_SRC}/Z2AudioCS/SpkSound.cpp"
  "${DECOMP_SRC}/Z2AudioCS/SpkSpeakerCtrl.cpp"
  "${DECOMP_SRC}/Z2AudioCS/SpkSystem.cpp"
  "${DECOMP_SRC}/Z2AudioCS/SpkTable.cpp"
  "${DECOMP_SRC}/Z2AudioCS/SpkWave.cpp"
  "${DECOMP_SRC}/Z2AudioCS/Z2AudioCS.cpp"

  # The higher-level Z2 audio runtime depends on substantial JAudio/JAU
  # behavior that is not implemented in the host stubs yet. Defer the entire
  # subsystem so the fast-port branch can keep probing other game systems.
  "${DECOMP_SRC}/Z2AudioLib/Z2Audience.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2AudioArcLoader.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2AudioMgr.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2Calc.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2Creature.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2DebugSys.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2EnvSeMgr.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2FxLineMgr.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2LinkMgr.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2Param.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2SceneMgr.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2SeMgr.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2SeqMgr.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2SoundHandles.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2SoundInfo.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2SoundMgr.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2SoundObjMgr.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2SoundObject.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2SoundPlayer.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2SoundStarter.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2SpeechMgr2.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2StatusMgr.cpp"
  "${DECOMP_SRC}/Z2AudioLib/Z2WolfHowlMgr.cpp"

  # Hardware debugger / emulator integration sources are not relevant to the
  # native port and drag C-only SDK assumptions into the build.
  "${DECOMP_SRC}/NdevExi2A/DebuggerDriver.c"
  "${DECOMP_SRC}/NdevExi2A/exi2.c"
  "${DECOMP_SRC}/REL/executor.c"
  "${DECOMP_SRC}/amcstubs/AmcExi2Stubs.c"
  "${DECOMP_SRC}/lingcod/LingcodPatch.c"
  "${DECOMP_SRC}/odemuexi2/DebuggerDriver.c"
  "${DECOMP_SRC}/odenotstub/odenotstub.c"
)

list(REMOVE_ITEM GAME_SOURCES ${TP_PORT_EXCLUDED_GAME_SOURCES})
list(SORT GAME_SOURCES)

# Always compile the port bootstrap alongside the decomp sources.
list(APPEND GAME_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

list(LENGTH GAME_SOURCES TP_PORT_GAME_SOURCE_COUNT)
list(LENGTH TP_PORT_EXCLUDED_GAME_SOURCES TP_PORT_EXCLUDED_SOURCE_COUNT)

message(STATUS
  "TP PC port broad-compile experiment: ${TP_PORT_GAME_SOURCE_COUNT} game sources, "
  "${TP_PORT_EXCLUDED_SOURCE_COUNT} explicit exclusions"
)
