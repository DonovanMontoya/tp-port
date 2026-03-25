/**
 * src/port/rarc.cpp
 * RARC / JKR archive format reader.
 *
 * RARC (.arc / .rarc) is the container format used by most GC/Wii Nintendo
 * games.  All multi-byte fields are big-endian.
 *
 * Header layout (0x40 bytes):
 *   0x00  u32  magic ("RARC")
 *   0x04  u32  file size
 *   0x08  u32  header size (always 0x20 for the main header)
 *   0x0C  u32  data offset (relative to header start)
 *   0x10  u32  data length
 *   0x14  u32  mram preload size
 *   0x18  u32  aram preload size
 *   0x1C  u32  reserved
 *
 * Information block at 0x20:
 *   0x20  u32  num_dir_nodes
 *   0x24  u32  dir_nodes_offset      (relative to info block start = 0x20)
 *   0x28  u32  num_file_entries
 *   0x2C  u32  file_entries_offset
 *   0x30  u32  string_table_size
 *   0x34  u32  string_table_offset
 *   0x38  u16  num_files (redundant)
 *   0x3A  u16  reserved
 *   0x3C  u32  reserved
 *
 * Dir node (0x10 bytes each):
 *   0x00  u32  type (4-char tag: "ROOT", "user name", ...)
 *   0x04  u32  name_offset (in string table)
 *   0x08  u16  name_hash
 *   0x0A  u16  num_entries
 *   0x0C  u32  first_entry_idx
 *
 * File entry (0x14 bytes each):
 *   0x00  u16  file_id (0xFFFF = directory)
 *   0x02  u16  name_hash
 *   0x04  u8   flags  (0x01 = file, 0x02 = dir)
 *   0x05  u8   [3]  (part of 24-bit name_offset)
 *   0x08  u32  data_offset  (file: relative to data region; dir: node index)
 *   0x0C  u32  data_size
 *   0x10  u32  reserved
 */

#include "port/port.h"
#include "port/rarc.h"
#include "port/endian.h"

#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <unordered_map>

using namespace tp::endian;

namespace tp::rarc {

// -----------------------------------------------------------------------
// Internal archive structure
// -----------------------------------------------------------------------
struct FileEntry {
    std::string name;
    bool        isDir;
    uint32_t    dataOffset;  // relative to data section
    uint32_t    dataSize;
    int         dirNodeIdx;  // for directories
};

struct RARCArchive {
    std::vector<uint8_t> data;    // entire file in memory
    uint32_t             dataOff; // offset of data region within data[]
    std::vector<FileEntry> files;
    std::unordered_map<std::string, int> nameIndex;  // lowercase path → file index
};

// -----------------------------------------------------------------------
// Parsing
// -----------------------------------------------------------------------
static bool ParseArchive(RARCArchive& arc) {
    const uint8_t* p = arc.data.data();
    if (arc.data.size() < 0x40) return false;

    // Magic check
    if (memcmp(p, "RARC", 4) != 0) return false;

    uint32_t headerDataOff = read_be32(p + 0x0C); // data offset from header start
    arc.dataOff = 0x20 + headerDataOff;            // absolute in our buffer

    // Information block at 0x20
    const uint8_t* info = p + 0x20;
    uint32_t numDirs       = read_be32(info + 0x00);
    uint32_t dirOff        = read_be32(info + 0x04);
    uint32_t numFiles      = read_be32(info + 0x08);
    uint32_t fileOff       = read_be32(info + 0x0C);
    uint32_t strOff        = read_be32(info + 0x14);

    const uint8_t* dirBase  = info + dirOff;
    const uint8_t* fileBase = info + fileOff;
    const uint8_t* strBase  = info + strOff;

    // Build file list by walking directory nodes
    // We do a simple linear scan of all file entries
    arc.files.reserve(numFiles);

    // Map dir node index → path prefix
    std::vector<std::string> dirPaths(numDirs);
    // First pass: read dir names
    for (uint32_t d = 0; d < numDirs; d++) {
        const uint8_t* dn = dirBase + d * 0x10;
        uint32_t nameOff = read_be32(dn + 0x04);
        const char* nm = reinterpret_cast<const char*>(strBase + nameOff);
        dirPaths[d] = (d == 0) ? "" : (dirPaths[0] + "/" + nm);
    }

    // Second pass: enumerate file entries
    for (uint32_t d = 0; d < numDirs; d++) {
        const uint8_t* dn = dirBase + d * 0x10;
        uint16_t nEntries  = read_be16(dn + 0x0A);
        uint32_t firstIdx  = read_be32(dn + 0x0C);
        const std::string& prefix = dirPaths[d];

        for (uint32_t e = 0; e < nEntries; e++) {
            const uint8_t* fe = fileBase + (firstIdx + e) * 0x14;
            uint16_t fileId   = read_be16(fe + 0x00);
            uint8_t  flags    = fe[0x04];
            uint32_t nameOff  = (uint32_t)fe[0x05] << 16 | read_be16(fe + 0x06);
            uint32_t dataOffset = read_be32(fe + 0x08);
            uint32_t dataSize   = read_be32(fe + 0x0C);

            const char* nm = reinterpret_cast<const char*>(strBase + nameOff);
            if (strcmp(nm, ".") == 0 || strcmp(nm, "..") == 0) continue;

            std::string path = prefix.empty() ? nm : (prefix + "/" + nm);
            // Lowercase path for case-insensitive lookup
            std::string lpath = path;
            for (char& c : lpath) c = (char)tolower(c);

            FileEntry entry;
            entry.name       = path;
            entry.isDir      = (flags & 0x02) != 0;
            entry.dataOffset = dataOffset;
            entry.dataSize   = dataSize;
            entry.dirNodeIdx = entry.isDir ? (int)dataOffset : -1;

            int idx = (int)arc.files.size();
            arc.files.push_back(entry);
            arc.nameIndex[lpath] = idx;
        }
    }
    return true;
}

// -----------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------

Archive* Open(const void* rawData, size_t size) {
    auto* arc = new RARCArchive();
    arc->data.assign((const uint8_t*)rawData, (const uint8_t*)rawData + size);
    if (!ParseArchive(*arc)) {
        delete arc;
        return nullptr;
    }
    return (Archive*)arc;
}

Archive* OpenFile(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return nullptr;
    fseek(f, 0, SEEK_END);
    size_t sz = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    auto* arc = new RARCArchive();
    arc->data.resize(sz);
    fread(arc->data.data(), 1, sz, f);
    fclose(f);
    if (!ParseArchive(*arc)) {
        delete arc;
        return nullptr;
    }
    return (Archive*)arc;
}

void Close(Archive* handle) {
    delete (RARCArchive*)handle;
}

const void* GetFile(Archive* handle, const char* path, size_t* outSize) {
    if (!handle || !path) return nullptr;
    auto* arc = (RARCArchive*)handle;
    std::string lpath = path;
    for (char& c : lpath) c = (char)tolower(c);
    auto it = arc->nameIndex.find(lpath);
    if (it == arc->nameIndex.end()) return nullptr;
    const FileEntry& fe = arc->files[it->second];
    if (fe.isDir) return nullptr;
    if (outSize) *outSize = fe.dataSize;
    return arc->data.data() + arc->dataOff + fe.dataOffset;
}

bool HasFile(Archive* handle, const char* path) {
    if (!handle || !path) return false;
    auto* arc = (RARCArchive*)handle;
    std::string lpath = path;
    for (char& c : lpath) c = (char)tolower(c);
    return arc->nameIndex.count(lpath) > 0;
}

} // namespace tp::rarc
