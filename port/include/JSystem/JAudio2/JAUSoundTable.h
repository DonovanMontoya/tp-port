#pragma once
/**
 * port/include/JSystem/JAudio2/JAUSoundTable.h
 * PC port stub — JAUSoundTable and related structs.
 * Preserves the full type structure but init/load do nothing.
 */
#include "port/types.h"

// ---------------------------------------------------------------------------
// Forward types used in the sound table
// ---------------------------------------------------------------------------
typedef u32 JAISoundID;

// Minimal JASGlobalInstance (if JAudio2/JASGadget.h is not included first)
#ifndef JASGLOBALINSTANCE_DEFINED
#define JASGLOBALINSTANCE_DEFINED
template <typename T>
struct JASGlobalInstance {
    static T* getInstance()       { return s_instance; }
    static void setInstance(T* p) { s_instance = p; }
protected:
    JASGlobalInstance(bool /*primary*/) {}
    static T* s_instance;
};
template <typename T> T* JASGlobalInstance<T>::s_instance = nullptr;
#endif

// ---------------------------------------------------------------------------
// Sound table item
// ---------------------------------------------------------------------------
struct JAUSoundTableItem {
    u8  mPriority;
    u8  field_0x1;
    u16 mResourceId;
    u32 field_0x4;
    f32 field_0x8;
};

// ---------------------------------------------------------------------------
// Sub-structures
// ---------------------------------------------------------------------------
struct JAUSoundTableRoot {
    static inline u32 magicNumber() { return 0x5420; }
    u32 mSectionNumber;
    u32 mSectionOffsets[0];
};

struct JAUSoundTableSection {
    int getGroupOffset(int index) const {
        if (index < 0 || index >= (int)mNumGroups) return 0;
        return mGroupOffsets[index];
    }
    u32 mNumGroups;
    u32 mGroupOffsets[0];
};

struct JAUSoundTableGroup {
    u8 getTypeID(int index) const {
        if (index < 0 || index >= (int)mNumItems) return 0xff;
        return mTypeIds[index * 4];
    }
    u32 getItemOffset(int index) const {
        if (index < 0 || index >= (int)mNumItems) return 0;
        return *(const u32*)(mTypeIds + index * 4) & 0xffffff;
    }
    u32 mNumItems;
    u32 field_0x4;
    u8  mTypeIds[0];
};

// ---------------------------------------------------------------------------
// Generic table wrapper
// ---------------------------------------------------------------------------
template<typename Root, typename Section, typename Group, typename>
struct JAUSoundTable_ {
    JAUSoundTable_() : field_0x0(nullptr), field_0x4(nullptr) {}

    void reset() { field_0x0 = nullptr; field_0x4 = nullptr; }

    void init(const void* data) {
        field_0x0 = data;
        if (!field_0x0) return;
        if (*(const u32*)field_0x0 + 0xbdad0000 != Root::magicNumber()) {
            field_0x0 = nullptr;
        } else {
            field_0x4 = (Root*)((const u8*)field_0x0 + *((const u32*)field_0x0 + 3));
        }
    }

    Section* getSection(int index) const {
        if (!field_0x4 || index < 0 || (u32)index >= field_0x4->mSectionNumber)
            return nullptr;
        u32 off = field_0x4->mSectionOffsets[index];
        if (!off) return nullptr;
        return (Section*)((const u8*)field_0x0 + off);
    }

    Group* getGroup(Section* sec, int index) const {
        if (!sec || index < 0 || (u32)index >= sec->mNumGroups) return nullptr;
        u32 off = sec->getGroupOffset(index);
        if (!off) return nullptr;
        return (Group*)((const u8*)field_0x0 + off);
    }

    const void* field_0x0;
    Root*        field_0x4;
};

// ---------------------------------------------------------------------------
// JAUSoundTable
// ---------------------------------------------------------------------------
struct JAUSoundTable : public JASGlobalInstance<JAUSoundTable> {
    JAUSoundTable(bool primary) : JASGlobalInstance<JAUSoundTable>(primary) {
        JASGlobalInstance<JAUSoundTable>::setInstance(this);
    }
    ~JAUSoundTable() {}

    void init(void const* data)  { field_0x0.init(data); }
    u8   getTypeID(JAISoundID)   const { return 0xff; }
    JAUSoundTableItem* getData(JAISoundID) const { return nullptr; }

    JAUSoundTableItem* getItem(JAUSoundTableGroup* group, int index) const {
        if (!group) return nullptr;
        u32 off = group->getItemOffset(index);
        if (!off) return nullptr;
        return (JAUSoundTableItem*)((const u8*)field_0x0.field_0x0 + off);
    }

    const void* getResource() const { return field_0x0.field_0x0; }
    bool        isValid()     const { return field_0x0.field_0x0 != nullptr; }

    JAUSoundTable_<JAUSoundTableRoot, JAUSoundTableSection, JAUSoundTableGroup, void> field_0x0;
};

// ---------------------------------------------------------------------------
// JAUSoundNameTable
// ---------------------------------------------------------------------------
struct JAUSoundNameTableRoot {
    static inline u32 magicNumber() { return 0x544e; }
    u32 mSectionNumber;
    u32 mSectionOffsets[0];
};
struct JAUSoundNameTableSection {};
struct JAUSoundNameTableGroup   {};

struct JAUSoundNameTable : public JASGlobalInstance<JAUSoundNameTable> {
    JAUSoundNameTable(bool primary) : JASGlobalInstance<JAUSoundNameTable>(primary) {
        JASGlobalInstance<JAUSoundNameTable>::setInstance(this);
    }
    ~JAUSoundNameTable() {}
    void init(void const* data) { field_0x0.init(data); }

    JAUSoundTable_<JAUSoundNameTableRoot, JAUSoundNameTableSection, JAUSoundNameTableGroup, void> field_0x0;
};
