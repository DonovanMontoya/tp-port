#pragma once
/**
 * PC stub for dolphin/dvd.h
 * Maps GameCube DVD file I/O to native filesystem access.
 * The game's files are extracted from the ISO by tools/extract_iso.py.
 * Implementation: src/dolphin/dvd.cpp
 */
#include "port/types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Result codes
#define DVD_RESULT_GOOD         0
#define DVD_RESULT_FATAL_ERROR -1
#define DVD_RESULT_IGNORED     -2
#define DVD_RESULT_CANCELED    -6

// State codes
#define DVD_STATE_FATAL_ERROR   -1
#define DVD_STATE_END            0
#define DVD_STATE_BUSY           1
#define DVD_STATE_WAITING        2
#define DVD_STATE_COVER_CLOSED   3
#define DVD_STATE_NO_DISK        4
#define DVD_STATE_COVER_OPEN     5
#define DVD_STATE_WRONG_DISK     6
#define DVD_STATE_MOTOR_STOPPED  7

#define DVD_MIN_TRANSFER_SIZE 32

typedef struct DVDDiskID {
    char gameName[4];
    char company[2];
    u8   diskNumber;
    u8   gameVersion;
    u8   streaming;
    u8   streamingBufSize;
    u8   padding[22];
} DVDDiskID;

typedef struct DVDCommandBlock DVDCommandBlock;
typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock* block);

struct DVDCommandBlock {
    DVDCommandBlock* next;
    DVDCommandBlock* prev;
    u32              command;
    s32              state;
    u32              offset;
    u32              length;
    void*            addr;
    u32              currTransferSize;
    u32              transferredSize;
    DVDDiskID*       id;
    DVDCBCallback    callback;
    void*            userData;
};

typedef struct DVDFileInfo {
    DVDCommandBlock cb;
    u32             startAddr;   // offset into the game file tree
    u32             length;
    // PC-only: backing file handle (FILE*)
    void*           _fp;
} DVDFileInfo;

typedef struct DVDDir {
    u32   entryNum;
    u32   location;
    u32   next;
} DVDDir;

typedef struct DVDDirEntry {
    u32  entryNum;
    BOOL isDir;
    char name[128];
} DVDDirEntry;

// Core API
void  DVDInit(void);
void  DVDQuit(void);
BOOL  DVDHasGameData(void);
const char* DVDGetGameDataPath(void);
s32   DVDConvertPathToEntrynum(const char* path);
const char* DVDGetPathFromEntrynum(s32 entrynum);
BOOL  DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo);
BOOL  DVDOpen(const char* fileName, DVDFileInfo* fileInfo);
BOOL  DVDClose(DVDFileInfo* fileInfo);
s32   DVDRead(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset);
s32   DVDReadPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, s32 prio);
BOOL  DVDReadAsync(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                   DVDCBCallback callback);
BOOL  DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                       DVDCBCallback callback, s32 prio);
BOOL  DVDCancelAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32   DVDGetFileSize(DVDFileInfo* fileInfo);

BOOL  DVDOpenDir(const char* dirName, DVDDir* dir);
BOOL  DVDReadDir(DVDDir* dir, DVDDirEntry* entry);
BOOL  DVDCloseDir(DVDDir* dir);
BOOL  DVDChangeDir(const char* dirName);
BOOL  DVDChangeDiskAsync(DVDDiskID* id, DVDCBCallback callback);

// Disk ID access
DVDDiskID* DVDGetCurrentDiskID(void);

// Low-level helpers
s32  DVDGetDriveStatus(void);
BOOL DVDCheckDisk(void);

#ifdef __cplusplus
}
#endif
