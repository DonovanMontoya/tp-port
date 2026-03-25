#!/usr/bin/env python3
"""
tools/extract_iso.py
Extract game files from a Twilight Princess GameCube ISO (GCM format)
into a directory tree that the PC port can use at runtime.

Usage:
    python tools/extract_iso.py <path/to/game.iso> [--out <outdir>]

Output layout:
    <outdir>/
        sys/
            main.dol       ← unused by port but extracted for reference
            fst.bin
            apploader.img
            boot.bin
        files/
            ... (all game files, path hierarchy preserved)

Dependencies:
    pip install py-gcn-iso  OR  use the standalone parser below (no deps).
"""

import argparse
import os
import struct
import sys
from pathlib import Path


# ---------------------------------------------------------------------------
# GCM / ISO parser (no external dependencies)
# ---------------------------------------------------------------------------

class GCMReader:
    # Boot block offsets (all big-endian)
    BOOT_MAGIC        = 0x00
    GAME_ID_LEN       = 6
    DVD_MAGIC_OFFSET  = 0x1C
    DVD_MAGIC_VALUE   = 0xC2339F3D
    FST_OFFSET_OFF    = 0x0424
    FST_SIZE_OFF      = 0x0428
    DOL_OFFSET_OFF    = 0x0420
    APPLOADER_OFF     = 0x2440
    FST_ENTRY_SIZE    = 12

    def __init__(self, iso_path: str):
        self.path = iso_path
        self.f    = open(iso_path, "rb")
        self._validate()

    def _validate(self):
        self.f.seek(self.DVD_MAGIC_OFFSET)
        magic = struct.unpack(">I", self.f.read(4))[0]
        if magic != self.DVD_MAGIC_VALUE:
            raise ValueError(f"Not a valid GCM/ISO (bad DVD magic: {magic:#010x})")

    def _read_u32(self, offset: int) -> int:
        self.f.seek(offset)
        return struct.unpack(">I", self.f.read(4))[0]

    def _read_cstr(self, offset: int) -> str:
        self.f.seek(offset)
        chars = []
        while True:
            b = self.f.read(1)
            if not b or b == b"\x00":
                break
            chars.append(b.decode("latin-1"))
        return "".join(chars)

    def game_id(self) -> str:
        self.f.seek(0)
        return self.f.read(6).decode("ascii", errors="replace")

    def extract_all(self, out_dir: Path):
        out_dir = Path(out_dir)

        # ── sys/ directory ────────────────────────────────────────────
        sys_dir = out_dir / "sys"
        sys_dir.mkdir(parents=True, exist_ok=True)

        # boot.bin (first 0x440 bytes)
        self._extract_raw(0, 0x440, sys_dir / "boot.bin")

        # apploader (starts at 0x2440)
        self.f.seek(0x2454)  # size field within apploader header
        al_size = struct.unpack(">I", self.f.read(4))[0]
        al_trailer = struct.unpack(">I", self.f.read(4))[0]
        al_total = 0x20 + al_size + al_trailer  # header + code + trailer
        self._extract_raw(0x2440, al_total, sys_dir / "apploader.img")

        # main.dol
        dol_offset = self._read_u32(self.DOL_OFFSET_OFF)
        # DOL size: sum of sections (simplified — just copy up to FST start)
        fst_offset = self._read_u32(self.FST_OFFSET_OFF)
        dol_size   = fst_offset - dol_offset
        self._extract_raw(dol_offset, dol_size, sys_dir / "main.dol")

        # fst.bin
        fst_offset = self._read_u32(self.FST_OFFSET_OFF)
        fst_size   = self._read_u32(self.FST_SIZE_OFF)
        self._extract_raw(fst_offset, fst_size, sys_dir / "fst.bin")

        # ── files/ via FST ────────────────────────────────────────────
        fst_data = self._read_bytes(fst_offset, fst_size)
        files_dir = out_dir / "files"
        self._extract_fst(fst_data, files_dir)

    def _extract_fst(self, fst_data: bytes, files_dir: Path):
        """Walk the FST and extract every file."""
        # Root entry
        root_flags = fst_data[0]
        if root_flags != 1:
            raise ValueError("FST root is not a directory")
        num_entries = struct.unpack_from(">I", fst_data, 8)[0]
        string_table_off = num_entries * self.FST_ENTRY_SIZE

        def get_name(name_off: int) -> str:
            end = fst_data.index(b"\x00", string_table_off + name_off)
            return fst_data[string_table_off + name_off:end].decode("latin-1")

        dir_stack: list[tuple[int, Path]] = []  # (end_index, current_path)
        dir_stack.append((num_entries, files_dir))
        total = 0

        for i in range(1, num_entries):
            entry  = fst_data[i * self.FST_ENTRY_SIZE : (i + 1) * self.FST_ENTRY_SIZE]
            flags  = entry[0]
            name_off = struct.unpack_from(">I", entry, 0)[0] & 0x00FFFFFF
            name   = get_name(name_off)

            # Pop directories we've left
            while len(dir_stack) > 1 and i >= dir_stack[-1][0]:
                dir_stack.pop()

            current_dir = dir_stack[-1][1]

            if flags == 1:  # directory
                next_entry = struct.unpack_from(">I", entry, 8)[0]
                new_dir    = current_dir / name
                new_dir.mkdir(parents=True, exist_ok=True)
                dir_stack.append((next_entry, new_dir))
            else:            # file
                file_offset = struct.unpack_from(">I", entry, 4)[0]
                file_size   = struct.unpack_from(">I", entry, 8)[0]
                dest        = current_dir / name
                dest.parent.mkdir(parents=True, exist_ok=True)
                self._extract_raw(file_offset, file_size, dest)
                total += 1
                if total % 100 == 0:
                    print(f"  Extracted {total} files...", end="\r", flush=True)

        print(f"  Extracted {total} files total.          ")

    def _extract_raw(self, offset: int, size: int, dest: Path):
        if size == 0:
            dest.write_bytes(b"")
            return
        self.f.seek(offset)
        CHUNK = 1 << 20  # 1 MB chunks
        dest.parent.mkdir(parents=True, exist_ok=True)
        with open(dest, "wb") as out:
            remaining = size
            while remaining > 0:
                chunk = min(CHUNK, remaining)
                data  = self.f.read(chunk)
                out.write(data)
                remaining -= len(data)

    def _read_bytes(self, offset: int, size: int) -> bytes:
        self.f.seek(offset)
        return self.f.read(size)

    def close(self):
        self.f.close()


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description="Extract Twilight Princess (GCM) ISO into port gamedata/")
    parser.add_argument("iso", help="Path to game ISO (.iso / .gcm)")
    parser.add_argument("--out", default="gamedata",
                        help="Output directory (default: ./gamedata)")
    args = parser.parse_args()

    iso_path = Path(args.iso)
    if not iso_path.exists():
        print(f"ERROR: ISO not found: {iso_path}", file=sys.stderr)
        sys.exit(1)

    out_dir = Path(args.out)
    print(f"Extracting {iso_path} → {out_dir}/")

    try:
        reader = GCMReader(str(iso_path))
    except ValueError as e:
        print(f"ERROR: {e}", file=sys.stderr)
        sys.exit(1)

    gid = reader.game_id()
    print(f"Game ID : {gid}")
    if not gid.startswith("GZ2"):
        print("WARNING: expected GameCube Twilight Princess (GZ2E01/GZ2P01/GZ2J01)")

    reader.extract_all(out_dir)
    reader.close()

    print(f"\nDone. Set TP_DATA_PATH={out_dir.resolve()} before running TwilightPrincess.exe")
    print(f"  or place the gamedata/ folder next to the executable.")


if __name__ == "__main__":
    main()
