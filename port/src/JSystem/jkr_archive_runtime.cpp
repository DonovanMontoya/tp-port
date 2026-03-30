#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "JSystem/JKernel/JKRMemArchive.h"

#include "dolphin/dvd.h"
#include "port/endian.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

using tp::endian::read_be16;
using tp::endian::read_be32;

std::string normalizePath(const char* path) {
    if (!path) {
        return {};
    }
    std::string out(path);
    std::replace(out.begin(), out.end(), '\\', '/');
    if (!out.empty() && out.front() == '/') {
        out.erase(out.begin());
    }
    for (char& c : out) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return out;
}

struct RegisteredArchiveState {
    std::vector<u8> ownedData;
    std::unordered_map<std::string, u32> pathToIndex;
};

std::unordered_map<const JKRArchive*, RegisteredArchiveState> sArchiveRegistry;

bool isYaz0(const u8* data, size_t size) {
    return size >= 16 && std::memcmp(data, "Yaz0", 4) == 0;
}

bool decodeYaz0(const std::vector<u8>& src, std::vector<u8>& dst) {
    if (!isYaz0(src.data(), src.size())) {
        return false;
    }

    const u32 decodedSize = read_be32(src.data() + 4);
    dst.assign(decodedSize, 0);

    size_t srcPos = 16;
    size_t dstPos = 0;
    u8 code = 0;
    int bitsLeft = 0;

    while (dstPos < dst.size() && srcPos < src.size()) {
        if (bitsLeft == 0) {
            code = src[srcPos++];
            bitsLeft = 8;
        }

        if ((code & 0x80) != 0) {
            if (srcPos >= src.size()) {
                return false;
            }
            dst[dstPos++] = src[srcPos++];
        } else {
            if (srcPos + 1 >= src.size()) {
                return false;
            }

            const u8 byte1 = src[srcPos++];
            const u8 byte2 = src[srcPos++];
            u32 copyLen = byte1 >> 4;
            const u32 dist = ((static_cast<u32>(byte1 & 0x0F) << 8) | byte2) + 1;

            if (copyLen == 0) {
                if (srcPos >= src.size()) {
                    return false;
                }
                copyLen = static_cast<u32>(src[srcPos++]) + 0x12;
            } else {
                copyLen += 2;
            }

            if (dist > dstPos) {
                return false;
            }

            size_t copyPos = dstPos - dist;
            for (u32 i = 0; i < copyLen && dstPos < dst.size(); ++i) {
                dst[dstPos++] = dst[copyPos++];
            }
        }

        code <<= 1;
        --bitsLeft;
    }

    return dstPos == dst.size();
}

u32 fileSizeFromRarcHeader(const void* data) {
    if (!data) {
        return 0;
    }
    const u8* bytes = static_cast<const u8*>(data);
    if (std::memcmp(bytes, "Yaz0", 4) == 0) {
        return read_be32(bytes + 4);
    }
    if (std::memcmp(bytes, "RARC", 4) != 0) {
        return 0;
    }
    return read_be32(bytes + 4);
}

bool loadEntryBytes(s32 entryNum, std::vector<u8>& out) {
    DVDFileInfo fileInfo;
    if (!DVDFastOpen(entryNum, &fileInfo)) {
        return false;
    }

    const s32 size = DVDGetFileSize(&fileInfo);
    if (size <= 0) {
        DVDClose(&fileInfo);
        return false;
    }

    out.resize(static_cast<size_t>(size));
    const s32 read = DVDRead(&fileInfo, out.data(), size, 0);
    DVDClose(&fileInfo);
    return read == size;
}

class PortArchive final : public JKRArchive {
public:
    PortArchive(std::vector<u8>&& ownedData, JKRHeap* heap, s32 entryNum, EMountMode mountMode,
                EMountDirection mountDirection)
        : mOwnedData(std::move(ownedData)) {
        mHeap = heap ? heap : JKRGetCurrentHeap();
        mEntryNum = entryNum;
        mMountMode = static_cast<u8>(mountMode);
        mMountDirection = mountDirection;
    }

    ~PortArchive() override {
        delete mArcInfoBlock;
        delete[] mNodes;
        delete[] mFiles;
    }

    bool parse() {
        if (!parseArchiveBuffer(this, mOwnedData.data(), mOwnedData.size(), mOwnedData.data(),
                                mPathToIndex, mFullPaths, &mDataOffset)) {
            return false;
        }
        sArchiveRegistry[this].pathToIndex = mPathToIndex;
        return true;
    }

    SDIFileEntry* findByName(const char* path) const {
        const std::string normalized = normalizePath(path);
        auto it = mPathToIndex.find(normalized);
        if (it != mPathToIndex.end()) {
            return const_cast<SDIFileEntry*>(&mFiles[it->second]);
        }
        return nullptr;
    }

    u32 resourceSize(const void* ptr) const {
        for (u32 i = 0; i < countFile(); ++i) {
            const SDIFileEntry& entry = mFiles[i];
            if (!entry.isDirectory() && entry.data == ptr) {
                return entry.data_size;
            }
        }
        return 0;
    }

    static bool parseArchiveBuffer(JKRArchive* archive, const u8* data, size_t size,
                                   const u8* ownedBase,
                                   std::unordered_map<std::string, u32>& pathToIndex,
                                   std::vector<std::string>& fullPaths, u32* outDataOffset) {
        if (size < 0x40 || std::memcmp(data, "RARC", 4) != 0) {
            return false;
        }

        const u32 headerDataOff = read_be32(data + 0x0C);
        const u32 dataOffset = 0x20 + headerDataOff;
        if (dataOffset >= size) {
            return false;
        }

        const u8* info = data + 0x20;
        const u32 numDirs = read_be32(info + 0x00);
        const u32 dirOff = read_be32(info + 0x04);
        const u32 numFiles = read_be32(info + 0x08);
        const u32 fileOff = read_be32(info + 0x0C);
        const u32 stringLen = read_be32(info + 0x10);
        const u32 stringOff = read_be32(info + 0x14);

        archive->mArcInfoBlock = new SArcDataInfo();
        archive->mArcInfoBlock->num_nodes = numDirs;
        archive->mArcInfoBlock->node_offset = dirOff;
        archive->mArcInfoBlock->num_file_entries = numFiles;
        archive->mArcInfoBlock->file_entry_offset = fileOff;
        archive->mArcInfoBlock->string_table_length = stringLen;
        archive->mArcInfoBlock->string_table_offset = stringOff;

        archive->mNodes = new JKRArchive::SDIDirEntry[numDirs]();
        archive->mFiles = new JKRArchive::SDIFileEntry[numFiles]();
        archive->mStringTable = reinterpret_cast<const char*>(info + stringOff);

        const u8* dirBase = info + dirOff;
        const u8* fileBase = info + fileOff;

        for (u32 i = 0; i < numDirs; ++i) {
            const u8* dn = dirBase + i * 0x10;
            archive->mNodes[i].type = read_be32(dn + 0x00);
            archive->mNodes[i].name_offset = read_be32(dn + 0x04);
            archive->mNodes[i].field_0x8 = read_be16(dn + 0x08);
            archive->mNodes[i].num_entries = read_be16(dn + 0x0A);
            archive->mNodes[i].first_file_index = read_be32(dn + 0x0C);
        }

        for (u32 i = 0; i < numFiles; ++i) {
            const u8* fe = fileBase + i * 0x14;
            auto& out = archive->mFiles[i];
            out.file_id = read_be16(fe + 0x00);
            out.name_hash = read_be16(fe + 0x02);
            out.type_flags_and_name_offset =
                (static_cast<u32>(fe[0x04]) << 24) | (static_cast<u32>(fe[0x05]) << 16) |
                read_be16(fe + 0x06);
            out.data_offset = read_be32(fe + 0x08);
            out.data_size = read_be32(fe + 0x0C);
            out.data = out.isDirectory() ? nullptr : const_cast<u8*>(ownedBase + dataOffset + out.data_offset);
        }

        fullPaths.assign(numFiles, {});
        buildPathMaps(archive, 0, "", pathToIndex, fullPaths);
        if (outDataOffset) {
            *outDataOffset = dataOffset;
        }
        return true;
    }

    static void buildPathMaps(JKRArchive* archive, u32 dirIndex, const std::string& prefix,
                              std::unordered_map<std::string, u32>& pathToIndex,
                              std::vector<std::string>& fullPaths) {
        if (!archive->mArcInfoBlock || dirIndex >= archive->countDirectory()) {
            return;
        }

        const auto& dir = archive->mNodes[dirIndex];
        for (u32 i = 0; i < dir.num_entries; ++i) {
            const u32 fileIndex = dir.first_file_index + i;
            if (fileIndex >= archive->countFile()) {
                continue;
            }

            auto& entry = archive->mFiles[fileIndex];
            const char* name = archive->mStringTable + entry.getNameOffset();
            if (std::strcmp(name, ".") == 0 || std::strcmp(name, "..") == 0) {
                continue;
            }

            const std::string path = prefix.empty() ? std::string(name) : prefix + "/" + name;
            fullPaths[fileIndex] = path;
            pathToIndex.emplace(normalizePath(path.c_str()), fileIndex);
            pathToIndex.emplace(normalizePath(name), fileIndex);

            if (entry.isDirectory()) {
                buildPathMaps(archive, entry.data_offset, path, pathToIndex, fullPaths);
            }
        }
    }

private:
    std::vector<u8> mOwnedData;
    u32 mDataOffset = 0;
    std::vector<std::string> mFullPaths;
    std::unordered_map<std::string, u32> mPathToIndex;
};

PortArchive* asPortArchive(JKRArchive* archive) {
    return dynamic_cast<PortArchive*>(archive);
}

const PortArchive* asPortArchive(const JKRArchive* archive) {
    return dynamic_cast<const PortArchive*>(archive);
}

JKRArchive* mountArchiveFromBytes(std::vector<u8>&& bytes, JKRHeap* heap, s32 entryNum,
                                  JKRArchive::EMountMode mountMode,
                                  JKRArchive::EMountDirection mountDirection) {
    if (isYaz0(bytes.data(), bytes.size())) {
        std::vector<u8> decoded;
        if (!decodeYaz0(bytes, decoded)) {
            return nullptr;
        }
        bytes = std::move(decoded);
    }

    auto archive = std::make_unique<PortArchive>(std::move(bytes), heap, entryNum, mountMode,
                                                 mountDirection);
    if (!archive->parse()) {
        return nullptr;
    }
    return archive.release();
}

void* loadEntryToMainRam(s32 entryNum, u8* dst, u32 dstLen, JKRHeap* heap, u32 offset,
                         JKRCompression* comp, u32* retSize) {
    DVDFileInfo fileInfo;
    if (!DVDFastOpen(entryNum, &fileInfo)) {
        return nullptr;
    }

    const s32 fileSize = DVDGetFileSize(&fileInfo);
    if (fileSize <= 0 || offset >= static_cast<u32>(fileSize)) {
        DVDClose(&fileInfo);
        return nullptr;
    }

    const u32 available = static_cast<u32>(fileSize) - offset;
    const u32 readSize = dstLen != 0 ? std::min(dstLen, available) : available;
    if (retSize) {
        *retSize = readSize;
    }
    if (comp) {
        *comp = COMPRESSION_NONE;
    }

    void* buffer = dst;
    if (!buffer) {
        JKRHeap* allocHeap = heap ? heap : JKRGetCurrentHeap();
        buffer = allocHeap ? allocHeap->alloc(readSize, 0x20) : std::malloc(readSize);
    }

    if (!buffer) {
        DVDClose(&fileInfo);
        return nullptr;
    }

    const s32 read = DVDRead(&fileInfo, buffer, static_cast<s32>(readSize), static_cast<s32>(offset));
    DVDClose(&fileInfo);
    if (read != static_cast<s32>(readSize)) {
        if (!dst) {
            if (heap) {
                heap->free(buffer);
            } else {
                std::free(buffer);
            }
        }
        return nullptr;
    }

    return buffer;
}

}  // namespace

bool JKRArchive::getDirEntry(SDirEntry* entry, u32 index) const {
    if (!entry || !findIdxResource(index)) {
        return false;
    }

    const SDIFileEntry* file = &mFiles[index];
    entry->flags = static_cast<u8>(file->getFlags());
    entry->id = file->file_id;
    entry->name = mStringTable ? (mStringTable + file->getNameOffset()) : nullptr;
    return true;
}

void* JKRArchive::getIdxResource(u32 index) {
    SDIFileEntry* entry = findIdxResource(index);
    return (entry && !entry->isDirectory()) ? entry->data : nullptr;
}

void* JKRArchive::getResource(u16 resourceId) {
    for (u32 i = 0; i < countFile(); ++i) {
        SDIFileEntry& entry = mFiles[i];
        if (!entry.isDirectory() && entry.file_id == resourceId) {
            return entry.data;
        }
    }
    return nullptr;
}

u32 JKRArchive::readIdxResource(void* buffer, u32 size, u32 index) {
    void* resource = getIdxResource(index);
    const u32 resourceSize = getResSize(resource);
    if (!buffer || !resource || resourceSize == 0) {
        return 0;
    }
    const u32 copySize = std::min(size, resourceSize);
    std::memcpy(buffer, resource, copySize);
    return copySize;
}

u32 JKRArchive::readResource(void* buffer, u32 size, u16 resourceId) {
    void* resource = getResource(resourceId);
    const u32 resourceSize = getResSize(resource);
    if (!buffer || !resource || resourceSize == 0) {
        return 0;
    }
    const u32 copySize = std::min(size, resourceSize);
    std::memcpy(buffer, resource, copySize);
    return copySize;
}

u32 JKRArchive::countResource() const {
    u32 count = 0;
    for (u32 i = 0; i < countFile(); ++i) {
        if (isFileEntry(i)) {
            ++count;
        }
    }
    return count;
}

bool JKRArchive::isFileEntry(u32 index) const {
    return index < countFile() && !mFiles[index].isDirectory();
}

JKRArchive::SDIFileEntry* JKRArchive::findNameResource(const char* path) const {
    auto it = sArchiveRegistry.find(this);
    if (it == sArchiveRegistry.end()) {
        const PortArchive* archive = asPortArchive(this);
        return archive ? archive->findByName(path) : nullptr;
    }
    const std::string normalized = normalizePath(path);
    auto pathIt = it->second.pathToIndex.find(normalized);
    return pathIt != it->second.pathToIndex.end() ? &mFiles[pathIt->second] : nullptr;
}

JKRArchive::SDIFileEntry* JKRArchive::findIdxResource(u32 index) const {
    return index < countFile() ? &mFiles[index] : nullptr;
}

void* JKRArchive::getResource(const char* path) {
    SDIFileEntry* entry = findNameResource(path);
    return (entry && !entry->isDirectory()) ? entry->data : nullptr;
}

void* JKRArchive::getResource(u32 /*tag*/, const char* path) {
    return getResource(path);
}

u32 JKRArchive::readResource(void* buffer, u32 size, const char* path) {
    void* resource = getResource(path);
    const u32 resourceSize = getResSize(resource);
    if (!buffer || !resource || resourceSize == 0) {
        return 0;
    }
    const u32 copySize = std::min(size, resourceSize);
    std::memcpy(buffer, resource, copySize);
    return copySize;
}

u32 JKRArchive::readResource(void* buffer, u32 size, u32 /*tag*/, const char* path) {
    return readResource(buffer, size, path);
}

u32 JKRArchive::getResSize(const void* resource) const {
    if (!resource) {
        return 0;
    }
    for (u32 i = 0; i < countFile(); ++i) {
        const SDIFileEntry& entry = mFiles[i];
        if (!entry.isDirectory() && entry.data == resource) {
            return entry.data_size;
        }
    }
    return 0;
}

u32 JKRArchive::countFile(const char* path) const {
    if (!path || !*path) {
        return countFile();
    }
    return findNameResource(path) ? 1 : 0;
}

void JKRArchive::unmount() {
    delete this;
}

JKRArchive* JKRArchive::mount(const char* path, EMountMode mountMode, JKRHeap* heap,
                              EMountDirection mountDirection) {
    const s32 entryNum = DVDConvertPathToEntrynum(path);
    if (entryNum < 0) {
        return nullptr;
    }
    return mount(entryNum, mountMode, heap, mountDirection);
}

JKRArchive* JKRArchive::mount(void* data, JKRHeap* heap, EMountDirection mountDirection) {
    const u32 size = fileSizeFromRarcHeader(data);
    if (size == 0) {
        return nullptr;
    }

    std::vector<u8> bytes(size);
    std::memcpy(bytes.data(), data, size);
    return mountArchiveFromBytes(std::move(bytes), heap, -1, MOUNT_MEM, mountDirection);
}

JKRArchive* JKRArchive::mount(s32 entryNum, EMountMode mountMode, JKRHeap* heap,
                              EMountDirection mountDirection) {
    std::vector<u8> bytes;
    if (!loadEntryBytes(entryNum, bytes)) {
        return nullptr;
    }
    return mountArchiveFromBytes(std::move(bytes), heap, entryNum, mountMode, mountDirection);
}

void* JKRArchive::getGlbResource(const char* path, JKRArchive* archive) {
    return archive ? archive->getResource(path) : nullptr;
}

void* JKRArchive::getGlbResource(u32 tag, const char* path, JKRArchive* archive) {
    return archive ? archive->getResource(tag, path) : nullptr;
}

void* JKRDvdRipper::loadToMainRAM(char const* path, u8* dst, JKRExpandSwitch /*sw*/, u32 dstLen,
                                  JKRHeap* heap, EAllocDirection /*dir*/, u32 offset,
                                  JKRCompression* comp, u32* retSize) {
    const s32 entryNum = DVDConvertPathToEntrynum(path);
    if (entryNum < 0) {
        return nullptr;
    }
    return loadToMainRAM(entryNum, dst, EXPAND_SWITCH_UNKNOWN1, dstLen, heap,
                         ALLOC_DIRECTION_FORWARD, offset, comp, retSize);
}

void* JKRDvdRipper::loadToMainRAM(s32 entryNum, u8* dst, JKRExpandSwitch /*sw*/, u32 dstLen,
                                  JKRHeap* heap, EAllocDirection /*dir*/, u32 offset,
                                  JKRCompression* comp, u32* retSize) {
    return loadEntryToMainRam(entryNum, dst, dstLen, heap, offset, comp, retSize);
}

void* JKRDvdRipper::loadToMainRAM(JKRDvdFile* /*file*/, u8* /*dst*/, JKRExpandSwitch /*sw*/,
                                  u32 /*dstLen*/, JKRHeap* /*heap*/, EAllocDirection /*dir*/,
                                  u32 /*offset*/, JKRCompression* /*comp*/, u32* /*retSize*/) {
    return nullptr;
}

JKRMemArchive::JKRMemArchive(s32 entryNum, JKRArchive::EMountDirection dir)
    : JKRArchive(entryNum, JKRArchive::MOUNT_MEM), mArcHeader(nullptr), mArchiveData(nullptr),
      mIsOpen(false) {
    open(entryNum, dir);
}

JKRMemArchive::JKRMemArchive(void* data, u32 size, JKRMemBreakFlag flag)
    : JKRArchive(-1, JKRArchive::MOUNT_MEM), mArcHeader(nullptr), mArchiveData(nullptr),
      mIsOpen(false) {
    open(data, size, flag);
}

JKRMemArchive::~JKRMemArchive() {
    sArchiveRegistry.erase(this);
    delete mArcInfoBlock;
    delete[] mNodes;
    delete[] mFiles;
}

bool JKRMemArchive::open(s32 entryNum, JKRArchive::EMountDirection dir) {
    std::vector<u8> bytes;
    if (!loadEntryBytes(entryNum, bytes)) {
        return false;
    }

    if (isYaz0(bytes.data(), bytes.size())) {
        std::vector<u8> decoded;
        if (!decodeYaz0(bytes, decoded)) {
            return false;
        }
        bytes = std::move(decoded);
    }

    auto& state = sArchiveRegistry[this];
    state.ownedData = std::move(bytes);
    state.pathToIndex.clear();
    std::vector<std::string> fullPaths;
    if (!PortArchive::parseArchiveBuffer(this, state.ownedData.data(), state.ownedData.size(),
                                         state.ownedData.data(), state.pathToIndex, fullPaths, nullptr)) {
        return false;
    }

    mMountDirection = dir;
    mArcHeader = reinterpret_cast<SArcHeader*>(state.ownedData.data());
    mArchiveData = state.ownedData.data();
    mIsOpen = true;
    return true;
}

bool JKRMemArchive::open(void* data, u32 size, JKRMemBreakFlag) {
    if (!data || size == 0) {
        return false;
    }

    std::vector<u8> bytes(static_cast<u8*>(data), static_cast<u8*>(data) + size);
    if (isYaz0(bytes.data(), bytes.size())) {
        std::vector<u8> decoded;
        if (!decodeYaz0(bytes, decoded)) {
            return false;
        }
        bytes = std::move(decoded);
    }

    auto& state = sArchiveRegistry[this];
    state.ownedData = std::move(bytes);
    state.pathToIndex.clear();
    std::vector<std::string> fullPaths;
    if (!PortArchive::parseArchiveBuffer(this, state.ownedData.data(), state.ownedData.size(),
                                         state.ownedData.data(), state.pathToIndex, fullPaths, nullptr)) {
        return false;
    }

    mArcHeader = reinterpret_cast<SArcHeader*>(state.ownedData.data());
    mArchiveData = state.ownedData.data();
    mIsOpen = true;
    return true;
}
