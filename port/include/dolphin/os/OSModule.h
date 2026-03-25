#pragma once
/**
 * port/include/dolphin/os/OSModule.h
 * PC port stub for OSModule (REL dynamic linking) types.
 * On PC there is no dynamic REL loader; types are provided for source compat.
 */
#include "port/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OS_MODULE_VERSION 3
typedef struct OSModuleHeader OSModuleHeader;
typedef u32 OSModuleID;
typedef struct OSModuleQueue OSModuleQueue;
typedef struct OSModuleLink OSModuleLink;
typedef struct OSModuleInfo OSModuleInfo;
typedef struct OSSectionInfo OSSectionInfo;
typedef struct OSImportInfo OSImportInfo;
typedef struct OSRel OSRel;

struct OSModuleQueue {
    OSModuleInfo* head;
    OSModuleInfo* tail;
};

struct OSModuleLink {
    OSModuleInfo* next;
    OSModuleInfo* prev;
};

struct OSModuleInfo {
    OSModuleID id;
    OSModuleLink link;
    u32 numSections;
    u32 sectionInfoOffset;
    u32 nameOffset;
    u32 nameSize;
    u32 version;
};

struct OSModuleHeader {
    OSModuleInfo info;
    u32 bssSize;
    u32 relOffset;
    u32 impOffset;
    u32 impSize;
    u8  prologSection;
    u8  epilogSection;
    u8  unresolvedSection;
    u8  bssSection;
    u32 prolog;
    u32 epilog;
    u32 unresolved;
    u32 align;
    u32 bssAlign;
    u32 fixSize;
};

struct OSSectionInfo {
    u32 offset;
    u32 size;
};

struct OSImportInfo {
    OSModuleID id;
    u32 offset;
};

struct OSRel {
    u16 offset;
    u8  type;
    u8  section;
    u32 addend;
};

static inline void OSSetStringTable(void* /*stringTable*/) {}
static inline BOOL OSLink(OSModuleInfo* /*newModule*/, void* /*bss*/) { return TRUE; }
static inline BOOL OSLinkFixed(OSModuleInfo* /*newModule*/, void* /*bss*/) { return TRUE; }
static inline BOOL OSUnlink(OSModuleInfo* /*oldModule*/) { return TRUE; }
static inline OSModuleInfo* OSSearchModule(void* /*ptr*/, u32* /*section*/, u32* /*offset*/) { return nullptr; }
static inline void OSNotifyLink(OSModuleInfo* /*module*/) {}
static inline void OSNotifyUnlink(OSModuleInfo* /*module*/) {}

#ifdef __cplusplus
}
#endif
