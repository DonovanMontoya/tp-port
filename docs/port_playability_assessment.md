# Twilight Princess PC Port: Playability Assessment

_Date: 2026-03-26_

## Executive summary

The `port/` tree is an early-to-mid bootstrap state: it can initialize a native window and build part of the game framework, but it is **not yet playable** as an end-user game.

Estimated distance to "first playable" (title screen + in-game frame loop with controllable Link and basic rendering/audio): **~30–45% complete**.

Estimated distance to "reasonably playable" (stable progression with core graphics/audio/input correctness): **~15–25% complete**.

## Evidence

### 1) Upstream repo is decompilation-focused, not a shipping port

The repository root explicitly states this project is a decompilation and "is not, and will not, produce a port"; the PC port work exists as an additional `port/` subtree. This means porting work must still replace many platform/hardware assumptions.

### 2) Port README documents major missing systems

`port/README.md` says the port layer compiles and opens a window, and lists high-priority missing tasks including:

- GX texture untiling / unswizzle
- TEV → GLSL translation (called out as largest task)
- Full GC ADPCM decode
- Display list record/replay
- Memory card/save implementation
- VI behavior, widescreen, and additional platform work

These are all central for visible, audible, and game-correct behavior.

### 3) Build graph is still selective and heavily staged

`port/cmake/game_sources.cmake` includes only a curated subset of decomp `.cpp` files and comments that sources are added incrementally as they become compatible.

Quantitatively:

- Total decomp `src/**/*.cpp`: **1085**
- Enabled in `port/cmake/game_sources.cmake`: **95**
- Ratio by file count: **~8.8%**

This ratio does not perfectly map to runtime completeness, but it strongly indicates an early integration stage.

### 4) Boot path exists, but runtime loop is not fully wired for gameplay

`port/src/main.cpp` currently calls `main01()` and still contains a separate stub loop with TODO for `fapGm_Execute()`, indicating loop wiring and runtime execution strategy are still in flux.

### 5) Numerous link/runtime stubs replace real subsystems

The port currently relies on broad no-op or placeholder implementations across many areas, e.g.:

- `port/src/m_Do/m_Do_stubs.cpp`
- `port/src/game_stubs.cpp`
- many API no-ops in `port/include/dolphin/*` and `port/include/revolution/*`

This is appropriate for incremental bring-up, but means feature behavior is not equivalent to hardware/game expectations.

### 6) Graphics backend explicitly acknowledges placeholder behavior

`port/src/dolphin/gx.cpp` describes the TEV→GLSL compiler as the largest remaining task and uses a basic shader path as scaffolding.

## Practical "distance to playability" interpretation

- **Current state:** technical bootstrap / engine bring-up.
- **Nearest milestone:** reliably entering and running a game frame loop with enough rendering fidelity + resource loading + input to display and navigate a known scene.
- **Main blockers:** graphics correctness (TEV + texture formats + display lists), content loading fidelity, and audio parity.

## Confidence and caveats

- Confidence in "not yet playable": **high** (directly supported by docs and code comments).
- Confidence in percentage estimate: **medium** (heuristic; based on enabled source coverage and subsystem criticality, not a full runtime test).
- Environment note: this assessment did not include a successful local configure/build because dependency fetch from GitHub was blocked by network policy in this environment.
