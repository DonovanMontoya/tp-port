# Twilight Princess PC Port

A native Windows (x86-64) port of *The Legend of Zelda: Twilight Princess*
(GameCube, GZ2E01) built on top of the
[zeldaret/tp](https://github.com/zeldaret/tp) decompilation.

> **You must own a legal copy of the game.**
> No game assets are included here.

---

## Architecture

```
port/
├── CMakeLists.txt          — top-level build
├── cmake/
│   ├── deps.cmake          — FetchContent (GLFW, OpenAL, SDL2, GLM)
│   └── game_sources.cmake  — decompiled .cpp files added incrementally
├── include/
│   ├── port/               — port-layer headers (types, endian, logging)
│   └── dolphin/            — PC stubs that shadow the GC SDK headers
├── src/
│   ├── main.cpp            — WinMain / main entry point
│   ├── dolphin/
│   │   ├── os.cpp          — OS thread/heap/time (→ std::thread)
│   │   ├── dvd.cpp         — DVD filesystem (→ native file I/O)
│   │   ├── gx.cpp          — GX graphics API  (→ OpenGL 4.5)
│   │   ├── ax.cpp          — AX audio          (→ OpenAL Soft)
│   │   └── pad.cpp         — PAD controller    (→ SDL2 + keyboard)
│   └── window/
│       └── window.cpp      — GLFW window + GL context
└── tools/
    └── extract_iso.py      — extract game files from your ISO
```

### Platform abstraction

The `include/dolphin/` directory **shadows** the decomp's `libs/dolphin/include/dolphin/`
headers.  CMake puts `port/include` first in the include path, so game source
files automatically pick up the PC stubs without modification.

Key mappings:

| GC subsystem | PC backend         |
|--------------|--------------------|
| GX           | OpenGL 4.5         |
| AX (audio)   | OpenAL Soft        |
| PAD          | SDL2 + XInput      |
| DVD          | Native filesystem  |
| OS threads   | `std::thread`      |
| OS memory    | `malloc` / `free`  |
| Memory card  | Save files on disk |

---

## Prerequisites

### Windows (recommended)

| Tool | Notes |
|------|-------|
| [Visual Studio 2022](https://visualstudio.microsoft.com/) or [LLVM/Clang](https://github.com/llvm/llvm-project/releases) | C++17 compiler |
| [CMake ≥ 3.20](https://cmake.org/download/) | Build system |
| [Git](https://git-scm.com/) | To fetch dependencies |
| Python 3 | ISO extraction tool |
| Your TP GCN ISO | GZ2E01 (NA), GZ2P01 (PAL), or GZ2J01 (JPN) |

GPU must support **OpenGL 4.5**.

### Linux / macOS

Same tools, substitute MSVC with GCC 11+ or Clang 14+.

---

## Build

### 1 — Extract game files

```bat
cd port
python tools/extract_iso.py "C:\path\to\TwilightPrincess.iso" --out gamedata
```

This creates `gamedata/files/` and `gamedata/sys/` from your disc image.

### 2 — Configure

```bat
cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
```

> On Linux/macOS add `-G Ninja` for faster builds.

CMake will automatically download GLFW, OpenAL Soft, SDL2, and GLM
via `FetchContent` (requires internet on first run).

### 3 — Build

```bat
cmake --build build --config RelWithDebInfo
```

### 4 — Run

```bat
set TP_DATA_PATH=gamedata
build\TwilightPrincess.exe
```

Or place the `gamedata/` folder next to the executable and omit `TP_DATA_PATH`.

---

## Porting progress

The port layer compiles and opens a window. This branch is experimenting with
an Animal Crossing-style workflow in `cmake/game_sources.cmake`:

1. Compile the decomp broadly by default.
2. Maintain a short exclusion list for TUs that still need platform work.
3. Fix compiler and linker fallout by improving the port layer and shrinking the exclusion list.
4. Re-enable gameplay entry points once `mDoMain_boot()` and `fapGm_Execute()` are stable.

That is the opposite of the original Twilight Princess port workflow, which
added game source files one by one only after they were known to compile.

### Known porting tasks (in rough priority order)

- [ ] **GX texture untiling** — GC textures are stored in 4×4 tiles; they must
      be unswizzled before uploading to GL. (`src/dolphin/gx.cpp`, `GXInitTexObj`)
- [ ] **TEV → GLSL compiler** — translate GX TEV stage configs to GLSL shader
      permutations. This is the largest single task. Reference: Dolphin emulator's
      `Source/Core/VideoCommon/`.
- [ ] **GC ADPCM full decode** — `src/dolphin/ax.cpp` has a partial decoder;
      needs looping, pitch-shift, and multi-channel support.
- [ ] **Display list recording/replay** — `GXCallDisplayList` needs to capture
      GL state and re-issue draw calls.
- [ ] **Memory card → save files** — `m_Do/m_Do_MemCard.cpp` uses CARD API.
- [ ] **Widescreen patch** — update the projection matrix for 16:9.
- [ ] **VI (Video Interface)** — stub `include/dolphin/vi.h`; mostly no-ops.
- [ ] **JSystem JKR archive loading** — `.arc` / `.rarc` files use big-endian
      headers; the endian wrappers in `port/include/port/endian.h` assist here.
- [ ] **PowerPC intrinsics** — some files use `__fabs`, `__frsqrte`, `asm` blocks;
      replace with `<cmath>` equivalents or compiler builtins.

---

## Controls

| Action       | Keyboard     | Controller          |
|--------------|--------------|---------------------|
| Move         | WASD         | Left stick          |
| Attack (A)   | Space        | A button            |
| Roll (B)     | X            | B button            |
| Action (X/Y) | Z / C        | X / Y buttons       |
| Start        | Enter        | Start               |
| D-pad        | Arrow keys   | D-pad               |
| Camera       | Mouse (TODO) | Right stick         |
| Z-target     | Tab (TODO)   | Z trigger           |

---

## Legal

This project contains no game assets and no reverse-engineered assembly.
It is a build system and platform abstraction layer on top of a clean-room
decompilation. You must supply your own legally obtained copy of the game.

The original decompilation: https://github.com/zeldaret/tp (CC0)
