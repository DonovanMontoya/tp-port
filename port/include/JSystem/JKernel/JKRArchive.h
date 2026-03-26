#pragma once
/**
 * port/include/JSystem/JKernel/JKRArchive.h
 * PC port stub — reduced JKRArchive surface for archive-backed game code.
 */

#include "JSystem/JKernel/JKRCompression.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "port/types.h"

class JKRFileFinder;
class JKRFileLoader {
public:
    virtual ~JKRFileLoader() {}
    virtual bool becomeCurrent(const char*) { return false; }
    virtual void* getResource(const char*) { return nullptr; }
    virtual void* getResource(u32, const char*) { return nullptr; }
    virtual u32 readResource(void*, u32, const char*) { return 0; }
    virtual u32 readResource(void*, u32, u32, const char*) { return 0; }
    virtual void removeResourceAll() {}
    virtual bool removeResource(void*) { return false; }
    virtual bool detachResource(void*) { return false; }
    virtual u32 getResSize(const void*) const { return 0; }
    virtual u32 countFile(const char*) const { return 0; }
    virtual JKRFileFinder* getFirstFile(const char*) const { return nullptr; }

    static bool removeResource(void* /*resource*/, JKRFileLoader* /*fileLoader*/) { return false; }
};

struct SArcHeader {
    u32 signature = 0;
    u32 file_length = 0;
    u32 header_length = 0;
    u32 file_data_offset = 0;
    u32 file_data_length = 0;
    u32 field_0x14 = 0;
    u32 field_0x18 = 0;
    u32 field_0x1c = 0;
};

struct SArcDataInfo {
    u32 num_nodes = 0;
    u32 node_offset = 0;
    u32 num_file_entries = 0;
    u32 file_entry_offset = 0;
    u32 string_table_length = 0;
    u32 string_table_offset = 0;
    u16 next_free_file_id = 0;
    bool sync_file_ids_and_indices = false;
    u8 field_1b[5] = {};
};

class JKRArchive : public JKRFileLoader {
public:
    struct SDirEntry {
        u8 flags = 0;
        u8 padding = 0;
        u16 id = 0;
        const char* name = nullptr;
    };

    struct SDIDirEntry {
        u32 type = 0;
        u32 name_offset = 0;
        u16 field_0x8 = 0;
        u16 num_entries = 0;
        u32 first_file_index = 0;
    };

    struct SDIFileEntry {
        u16 file_id = 0;
        u16 name_hash = 0;
        u32 type_flags_and_name_offset = 0;
        u32 data_offset = 0;
        u32 data_size = 0;
        void* data = nullptr;

        u32 getNameOffset() const { return type_flags_and_name_offset & 0xFFFFFF; }
        u16 getNameHash() const { return name_hash; }
        u32 getFlags() const { return type_flags_and_name_offset >> 24; }
        u32 getAttr() const { return getFlags(); }
        u16 getFileID() const { return file_id; }
        bool isDirectory() const { return (getFlags() & 0x02) != 0; }
        bool isCompressed() const { return (getFlags() & 0x04) != 0; }
        u8 getCompressFlag() const { return static_cast<u8>(getFlags() & 0x04); }
        bool isYAZ0Compressed() const { return (getFlags() & 0x80) != 0; }
        u32 getSize() const { return data_size; }
    };

    enum EMountMode {
        UNKNOWN_MOUNT_MODE = 0,
        MOUNT_MEM = 1,
        MOUNT_ARAM = 2,
        MOUNT_DVD = 3,
        MOUNT_COMP = 4,
    };

    enum EMountDirection {
        UNKNOWN_MOUNT_DIRECTION = 0,
        MOUNT_DIRECTION_HEAD = 1,
        MOUNT_DIRECTION_TAIL = 2,
    };

    JKRArchive() = default;
    JKRArchive(s32 entryNum, EMountMode mountMode)
        : mHeap(JKRGetCurrentHeap()), mMountMode(static_cast<u8>(mountMode)), mEntryNum(entryNum) {}
    virtual ~JKRArchive() {}

    bool getDirEntry(SDirEntry*, u32) const { return false; }
    void* getIdxResource(u32) { return nullptr; }
    void* getResource(u16) { return nullptr; }
    u32 readIdxResource(void*, u32, u32) { return 0; }
    u32 readResource(void*, u32, u16) { return 0; }
    u32 countResource() const { return 0; }
    u8 getFileAttribute(u32) const { return 0; }

    SDIFileEntry* findNameResource(const char*) const { return nullptr; }
    bool isSameName(const char*, u32, u16) const { return false; }
    SDIDirEntry* findResType(u32) const { return nullptr; }
    SDIDirEntry* findDirectory(const char*, u32) const { return nullptr; }
    SDIFileEntry* findTypeResource(u32, const char*) const { return nullptr; }
    SDIFileEntry* findFsResource(const char*, u32) const { return nullptr; }
    SDIFileEntry* findIdxResource(u32) const { return nullptr; }
    SDIFileEntry* findPtrResource(const void*) const { return nullptr; }
    SDIFileEntry* findIdResource(u16) const { return nullptr; }

    virtual bool becomeCurrent(const char*) override { return false; }
    virtual void* getResource(const char*) override { return nullptr; }
    virtual void* getResource(u32, const char*) override { return nullptr; }
    virtual u32 readResource(void*, u32, const char*) override { return 0; }
    virtual u32 readResource(void*, u32, u32, const char*) override { return 0; }
    virtual void removeResourceAll() override {}
    virtual bool removeResource(void*) override { return false; }
    virtual bool detachResource(void*) override { return false; }
    virtual u32 getResSize(const void*) const override { return 0; }
    virtual u32 countFile(const char*) const override { return 0; }
    virtual JKRFileFinder* getFirstFile(const char*) const override { return nullptr; }
    virtual u32 getExpandedResSize(const void* res) const { return getResSize(res); }
    virtual void* fetchResource(SDIFileEntry*, u32*) { return nullptr; }
    virtual void* fetchResource(void*, u32, SDIFileEntry*, u32*) { return nullptr; }
    virtual void setExpandSize(SDIFileEntry*, u32) {}
    virtual u32 getExpandSize(SDIFileEntry*) const { return 0; }
    virtual void unmount() {}

    u32 countFile() const { return mArcInfoBlock ? mArcInfoBlock->num_file_entries : 0; }
    s32 countDirectory() const { return mArcInfoBlock ? static_cast<s32>(mArcInfoBlock->num_nodes) : 0; }
    u8 getMountMode() const { return mMountMode; }

    static JKRArchive* check_mount_already(s32, JKRHeap*) { return nullptr; }
    static JKRArchive* mount(const char*, EMountMode, JKRHeap*, EMountDirection) {
        return nullptr;
    }
    static JKRArchive* mount(void*, JKRHeap*, EMountDirection) {
        return nullptr;
    }
    static JKRArchive* mount(s32, EMountMode, JKRHeap*, EMountDirection) {
        return nullptr;
    }
    static void* getGlbResource(const char*, JKRArchive*) { return nullptr; }
    static void* getGlbResource(u32, const char*, JKRArchive*) { return nullptr; }

    static JKRCompression convertAttrToCompressionType(int attr) {
        if (!(attr & 0x04)) {
            return COMPRESSION_NONE;
        } else if (attr & 0x80) {
            return COMPRESSION_YAZ0;
        } else {
            return COMPRESSION_YAY0;
        }
    }

    static u32 getCurrentDirID() { return sCurrentDirID; }
    static void setCurrentDirID(u32 dirID) { sCurrentDirID = dirID; }

    JKRHeap* mHeap = nullptr;
    u8 mMountMode = 0;
    u8 field_0x3d[3] = {};
    s32 mEntryNum = -1;
    SArcDataInfo* mArcInfoBlock = nullptr;
    SDIDirEntry* mNodes = nullptr;
    SDIFileEntry* mFiles = nullptr;
    s32* mExpandedSize = nullptr;
    const char* mStringTable = nullptr;
    u32 field_0x58 = 0;
    JKRCompression mCompression = COMPRESSION_NONE;
    EMountDirection mMountDirection = UNKNOWN_MOUNT_DIRECTION;

    static inline u32 sCurrentDirID = 0;
};

inline JKRCompression JKRConvertAttrToCompressionType(int attr) {
    return JKRArchive::convertAttrToCompressionType(attr);
}

inline void* JKRGetResource(u32 node, const char* path, JKRArchive* archive) {
    return JKRArchive::getGlbResource(node, path, archive);
}

inline void* JKRGetTypeResource(u32 tag, const char* name, JKRArchive* arc) {
    return JKRArchive::getGlbResource(tag, name, arc);
}

inline bool JKRRemoveResource(void* resource, JKRFileLoader* fileLoader) {
    return JKRFileLoader::removeResource(resource, fileLoader);
}

inline JKRArchive* JKRMountArchive(void* ptr, JKRHeap* heap, JKRArchive::EMountDirection mountDirection) {
    return JKRArchive::mount(ptr, heap, mountDirection);
}

inline void JKRUnmountArchive(JKRArchive* arc) {
    if (arc) {
        arc->unmount();
    }
}

inline u32 JKRReadIdxResource(void* buffer, u32 bufsize, u32 resIdx, JKRArchive* archive) {
    return archive ? archive->readIdxResource(buffer, bufsize, resIdx) : 0;
}
