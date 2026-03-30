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
#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

// -----------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------
static fs::path sGameDataDir;
static bool     sHasGameData = false;

static DVDDiskID sDiskID = {
    {'G','Z','2','E'}, {'0','1'}, 0, 0, 0, 0, {}
};

// -----------------------------------------------------------------------
// FST (File System Table) — maps GC path → host path
// -----------------------------------------------------------------------
static std::unordered_map<std::string, std::string> sFSTMap;
static std::unordered_map<std::string, s32> sPathToEntry;
static std::unordered_map<s32, std::string> sEntryToPath;

static bool HasExpectedGameDataLayout(const fs::path& root) {
    return fs::exists(root) && fs::exists(root / "files") && fs::exists(root / "sys");
}

static fs::path GetExecutableDir() {
#ifdef _WIN32
    char buffer[MAX_PATH];
    DWORD len = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (len > 0 && len < MAX_PATH) {
        return fs::path(buffer).parent_path();
    }
#endif
    return {};
}

static void BuildFSTMap(const fs::path& root) {
    // Walk the extracted files directory and build a name → absolute path map.
    // Game code uses paths like "/res/Stage/D_MN05/Stage.arc"
    fs::path filesDir = root / "files";
    if (!fs::exists(filesDir)) {
        tp::log::warn("DVD: game data dir '%s' has no 'files/' subdir", root.string().c_str());
        return;
    }
    sPathToEntry.clear();
    sEntryToPath.clear();
    sEntryToPath[0] = "/";
    s32 nextEntry = 1;

    for (auto& entry : fs::recursive_directory_iterator(filesDir)) {
        if (entry.is_regular_file()) {
            // Build virtual GC path: strip the filesDir prefix, prepend "/"
            std::string rel = "/" + fs::relative(entry.path(), filesDir).generic_string();
            sFSTMap[rel] = entry.path().string();
            sPathToEntry[rel] = nextEntry;
            sEntryToPath[nextEntry] = rel;
            ++nextEntry;
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

static s32 ResolveEntryNum(const char* gcPath) {
    auto it = sPathToEntry.find(gcPath);
    if (it != sPathToEntry.end()) {
        return it->second;
    }

    std::string lower = gcPath;
    for (char& c : lower) c = static_cast<char>(tolower(c));
    for (auto& [path, entry] : sPathToEntry) {
        std::string cmp = path;
        for (char& c : cmp) c = static_cast<char>(tolower(c));
        if (cmp == lower) {
            return entry;
        }
    }

    return -1;
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
    sGameDataDir.clear();
    sHasGameData = false;

    std::vector<fs::path> candidates;
    if (envPath && *envPath) {
        candidates.emplace_back(envPath);
    }
    candidates.push_back(fs::current_path() / "gamedata");
    candidates.push_back(fs::current_path() / "port" / "gamedata");
    fs::path exeDir = GetExecutableDir();
    if (!exeDir.empty()) {
        candidates.push_back(exeDir / "gamedata");
        candidates.push_back(exeDir.parent_path() / "gamedata");
        candidates.push_back(exeDir.parent_path() / "port" / "gamedata");
        candidates.push_back(exeDir.parent_path().parent_path() / "gamedata");
        candidates.push_back(exeDir.parent_path().parent_path() / "port" / "gamedata");
    }

    for (const fs::path& candidate : candidates) {
        if (HasExpectedGameDataLayout(candidate)) {
            sGameDataDir = candidate;
            sHasGameData = true;
            break;
        }
        if (sGameDataDir.empty() && fs::exists(candidate)) {
            sGameDataDir = candidate;
        }
    }

    if (!sHasGameData) {
        if (sGameDataDir.empty()) {
            sGameDataDir = fs::current_path() / "gamedata";
        }
        tp::log::error("DVD: game data directory not found: '%s'",
                       sGameDataDir.string().c_str());
        tp::log::error("     Run tools/extract_iso.py first, or set TP_DATA_PATH.");
    } else {
        BuildFSTMap(sGameDataDir);
        tp::log::info("DVD: using game data directory '%s'", sGameDataDir.string().c_str());
    }

    // Start async worker
    sAsyncRunning = true;
    sAsyncThread  = std::thread(AsyncWorker);
}

BOOL DVDHasGameData(void) {
    return sHasGameData ? TRUE : FALSE;
}

const char* DVDGetGameDataPath(void) {
    static std::string path;
    path = sGameDataDir.string();
    return path.c_str();
}

void DVDQuit(void) {
    sAsyncRunning = false;
    sAsyncCV.notify_all();
    if (sAsyncThread.joinable())
        sAsyncThread.join();
}

BOOL DVDOpen(const char* fileName, DVDFileInfo* fileInfo) {
    if (!fileInfo || !fileName) return FALSE;
    memset(fileInfo, 0, sizeof(*fileInfo));

    std::string hostPath = ResolvePath(fileName);
    if (hostPath.empty()) {
        tp::log::warn("DVD: file not found: %s", fileName);
        return FALSE;
    }

    tp::log::info("DVDOpen: %s -> %s", fileName, hostPath.c_str());

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

BOOL DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo) {
    auto it = sEntryToPath.find(entrynum);
    if (it == sEntryToPath.end() || entrynum == 0) {
        tp::log::warn("DVD: invalid fast-open entry: %d", entrynum);
        return FALSE;
    }
    tp::log::info("DVDFastOpen: %d -> %s", entrynum, it->second.c_str());
    return DVDOpen(it->second.c_str(), fileInfo);
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

s32 DVDConvertPathToEntrynum(const char* path) {
    if (!path) {
        return -1;
    }
    return ResolveEntryNum(path);
}

const char* DVDGetPathFromEntrynum(s32 entrynum) {
    static std::string path;
    auto it = sEntryToPath.find(entrynum);
    if (it == sEntryToPath.end()) {
        path.clear();
        return nullptr;
    }
    path = it->second;
    return path.c_str();
}

BOOL DVDOpenDir(const char* dirName, DVDDir* dir) {
    if (!dir || !dirName) return FALSE;
    (void)dirName;
    // TODO: implement directory iteration when needed
    return FALSE;
}

BOOL DVDReadDir(DVDDir* /*dir*/, DVDDirEntry* /*entry*/) { return FALSE; }
BOOL DVDCloseDir(DVDDir* /*dir*/) { return TRUE; }
BOOL DVDChangeDir(const char* /*dirName*/) { return TRUE; }

DVDDiskID* DVDGetCurrentDiskID(void) { return &sDiskID; }
s32  DVDGetDriveStatus(void) { return DVD_STATE_END; }
BOOL DVDCheckDisk(void) { return TRUE; }

BOOL DVDChangeDiskAsync(DVDDiskID* /*id*/, DVDCBCallback callback) {
    if (callback) callback(DVD_RESULT_GOOD, nullptr);
    return TRUE;
}
