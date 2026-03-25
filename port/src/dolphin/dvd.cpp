/**
 * src/dolphin/dvd.cpp
 * PC implementation of the GameCube DVD filesystem.
 *
 * The GC DVD API reads files from a specific offset on the disc.
 * On PC we map those reads to the extracted game file tree:
 *
 *   <GAME_DATA_DIR>/                 (set by TP_DATA_PATH env or auto-detected)
 *     sys/
 *       main.dol
 *       fst.bin         ← filesystem table (parsed at init)
 *       apploader.img
 *     files/
 *       ... (all game files extracted by tools/extract_iso.py)
 *
 * Async reads are dispatched on a dedicated worker thread, matching the
 * original's DVD thread architecture (see m_Do/m_Do_dvd_thread.cpp).
 */

#include "port/port.h"
#include "dolphin/dvd.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <filesystem>

namespace fs = std::filesystem;

// -----------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------
static fs::path sGameDataDir;

static DVDDiskID sDiskID = {
    "GZ2E", "01", 0, 0, 0, 0, {}
};

// -----------------------------------------------------------------------
// FST (File System Table) — maps GC path → host path
// -----------------------------------------------------------------------
static std::unordered_map<std::string, std::string> sFSTMap;

static void BuildFSTMap(const fs::path& root) {
    // Walk the extracted files directory and build a name → absolute path map.
    // Game code uses paths like "/res/Stage/D_MN05/Stage.arc"
    fs::path filesDir = root / "files";
    if (!fs::exists(filesDir)) {
        tp::log::warn("DVD: game data dir '%s' has no 'files/' subdir", root.string().c_str());
        return;
    }
    for (auto& entry : fs::recursive_directory_iterator(filesDir)) {
        if (entry.is_regular_file()) {
            // Build virtual GC path: strip the filesDir prefix, prepend "/"
            std::string rel = "/" + fs::relative(entry.path(), filesDir).generic_string();
            sFSTMap[rel] = entry.path().string();
        }
    }
    tp::log::info("DVD: indexed %zu files from '%s'", sFSTMap.size(), root.string().c_str());
}

static std::string ResolvePath(const char* gcPath) {
    auto it = sFSTMap.find(gcPath);
    if (it != sFSTMap.end()) return it->second;
    // Try case-insensitive search (game sometimes uses mixed case)
    std::string lower = gcPath;
    for (char& c : lower) c = static_cast<char>(tolower(c));
    for (auto& [k, v] : sFSTMap) {
        std::string kl = k;
        for (char& c : kl) c = static_cast<char>(tolower(c));
        if (kl == lower) return v;
    }
    return {};
}

// -----------------------------------------------------------------------
// Async DVD worker thread
// -----------------------------------------------------------------------
struct AsyncRequest {
    DVDFileInfo*  fileInfo;
    void*         addr;
    s32           length;
    s32           offset;
    DVDCBCallback callback;
};

static std::queue<AsyncRequest>    sAsyncQueue;
static std::mutex                  sAsyncMutex;
static std::condition_variable     sAsyncCV;
static std::thread                 sAsyncThread;
static bool                        sAsyncRunning = false;

static void AsyncWorker() {
    while (sAsyncRunning) {
        std::unique_lock<std::mutex> lock(sAsyncMutex);
        sAsyncCV.wait(lock, []{ return !sAsyncQueue.empty() || !sAsyncRunning; });
        if (!sAsyncRunning) break;

        AsyncRequest req = sAsyncQueue.front();
        sAsyncQueue.pop();
        lock.unlock();

        s32 result = DVDRead(req.fileInfo, req.addr, req.length, req.offset);
        if (req.callback) {
            req.callback(result >= 0 ? DVD_RESULT_GOOD : DVD_RESULT_FATAL_ERROR,
                         &req.fileInfo->cb);
        }
    }
}

// -----------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------
void DVDInit(void) {
    // Locate game data directory
    const char* envPath = getenv("TP_DATA_PATH");
    if (envPath && fs::exists(envPath)) {
        sGameDataDir = envPath;
    } else {
        // Look for a "gamedata" folder next to the executable
        sGameDataDir = fs::current_path() / "gamedata";
    }

    if (!fs::exists(sGameDataDir)) {
        tp::log::error("DVD: game data directory not found: '%s'",
                       sGameDataDir.string().c_str());
        tp::log::error("     Run tools/extract_iso.py first, or set TP_DATA_PATH.");
    } else {
        BuildFSTMap(sGameDataDir);
    }

    // Start async worker
    sAsyncRunning = true;
    sAsyncThread  = std::thread(AsyncWorker);
}

BOOL DVDOpen(const char* fileName, DVDFileInfo* fileInfo) {
    if (!fileInfo || !fileName) return FALSE;
    memset(fileInfo, 0, sizeof(*fileInfo));

    std::string hostPath = ResolvePath(fileName);
    if (hostPath.empty()) {
        tp::log::warn("DVD: file not found: %s", fileName);
        return FALSE;
    }

    FILE* fp = fopen(hostPath.c_str(), "rb");
    if (!fp) {
        tp::log::warn("DVD: cannot open: %s", hostPath.c_str());
        return FALSE;
    }

    fseek(fp, 0, SEEK_END);
    fileInfo->length = static_cast<u32>(ftell(fp));
    fseek(fp, 0, SEEK_SET);
    fileInfo->_fp = fp;
    fileInfo->cb.state = DVD_STATE_END;
    return TRUE;
}

BOOL DVDClose(DVDFileInfo* fileInfo) {
    if (!fileInfo || !fileInfo->_fp) return FALSE;
    fclose(static_cast<FILE*>(fileInfo->_fp));
    fileInfo->_fp = nullptr;
    return TRUE;
}

s32 DVDRead(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset) {
    if (!fileInfo || !fileInfo->_fp || !addr || length <= 0) return DVD_RESULT_FATAL_ERROR;
    FILE* fp = static_cast<FILE*>(fileInfo->_fp);
    if (fseek(fp, offset, SEEK_SET) != 0) return DVD_RESULT_FATAL_ERROR;
    size_t read = fread(addr, 1, static_cast<size_t>(length), fp);
    return static_cast<s32>(read);
}

s32 DVDReadPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, s32 /*prio*/) {
    return DVDRead(fileInfo, addr, length, offset);
}

BOOL DVDReadAsync(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                  DVDCBCallback callback)
{
    return DVDReadAsyncPrio(fileInfo, addr, length, offset, callback, 2);
}

BOOL DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                      DVDCBCallback callback, s32 /*prio*/)
{
    if (!fileInfo) return FALSE;
    {
        std::lock_guard<std::mutex> lock(sAsyncMutex);
        sAsyncQueue.push({fileInfo, addr, length, offset, callback});
    }
    sAsyncCV.notify_one();
    return TRUE;
}

BOOL DVDCancelAsync(DVDCommandBlock* /*block*/, DVDCBCallback callback) {
    if (callback) callback(DVD_RESULT_CANCELED, nullptr);
    return TRUE;
}

s32 DVDGetFileSize(DVDFileInfo* fileInfo) {
    if (!fileInfo) return 0;
    return static_cast<s32>(fileInfo->length);
}

BOOL DVDConvertPathToEntryNum(const char* path) {
    return !ResolvePath(path).empty() ? TRUE : FALSE;
}

BOOL DVDOpenDir(const char* dirName, DVDDir* dir) {
    if (!dir || !dirName) return FALSE;
    (void)dirName;
    // TODO: implement directory iteration when needed
    return FALSE;
}

BOOL DVDReadDir(DVDDir* /*dir*/, DVDDirEntry* /*entry*/) { return FALSE; }
BOOL DVDCloseDir(DVDDir* /*dir*/) { return TRUE; }

DVDDiskID* DVDGetCurrentDiskID(void) { return &sDiskID; }
s32  DVDGetDriveStatus(void) { return DVD_STATE_END; }
BOOL DVDCheckDisk(void) { return TRUE; }

BOOL DVDChangeDiskAsync(DVDDiskID* /*id*/, DVDCBCallback callback) {
    if (callback) callback(DVD_RESULT_GOOD, nullptr);
    return TRUE;
}
