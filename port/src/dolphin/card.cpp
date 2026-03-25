/**
 * src/dolphin/card.cpp
 * Memory Card API → PC filesystem save backend.
 *
 * On PC each card slot maps to a plain file:
 *   ./saves/card_A.bin   (CARD_SLOT_A = 0)
 *   ./saves/card_B.bin   (CARD_SLOT_B = 1)
 *
 * "Card files" are stored as a flat key-value store inside the .bin:
 *   [4-byte name-len][name bytes][4-byte data-len][data bytes] …
 *
 * Because the original CARDFileInfo has no filename field we maintain an
 * internal table: chan × fileNo → filename string.
 */

#include "port/port.h"
#include "dolphin/card.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <filesystem>

namespace fs = std::filesystem;

// -----------------------------------------------------------------------
// Internal state
// -----------------------------------------------------------------------

struct CardState {
    bool      mounted = false;
    bool      dirty   = false;
    std::string path;
    // fileNo → { name, data }
    std::vector<std::string>              names;
    std::unordered_map<std::string, int>  nameToNo;
    std::vector<std::vector<uint8_t>>     data;
};

static CardState s_cards[2];
static std::mutex s_cardMtx;

// -----------------------------------------------------------------------
// Serialise / deserialise
// -----------------------------------------------------------------------
static void SaveCard(CardState& c) {
    FILE* f = fopen(c.path.c_str(), "wb");
    if (!f) return;
    for (int i = 0; i < (int)c.names.size(); i++) {
        const std::string& nm = c.names[i];
        const std::vector<uint8_t>& d = c.data[i];
        uint32_t nlen = (uint32_t)nm.size();
        uint32_t dlen = (uint32_t)d.size();
        fwrite(&nlen, 4, 1, f);
        fwrite(nm.data(), 1, nlen, f);
        fwrite(&dlen, 4, 1, f);
        fwrite(d.data(), 1, dlen, f);
    }
    fclose(f);
    c.dirty = false;
}

static void LoadCard(CardState& c) {
    c.names.clear();
    c.nameToNo.clear();
    c.data.clear();
    FILE* f = fopen(c.path.c_str(), "rb");
    if (!f) return;
    while (true) {
        uint32_t nlen = 0;
        if (fread(&nlen, 4, 1, f) != 1) break;
        std::string nm(nlen, '\0');
        if (fread(nm.data(), 1, nlen, f) != nlen) break;
        uint32_t dlen = 0;
        if (fread(&dlen, 4, 1, f) != 1) break;
        std::vector<uint8_t> d(dlen);
        if (fread(d.data(), 1, dlen, f) != dlen) break;
        int no = (int)c.names.size();
        c.nameToNo[nm] = no;
        c.names.push_back(std::move(nm));
        c.data.push_back(std::move(d));
    }
    fclose(f);
}

// -----------------------------------------------------------------------
// Worker thread for async callbacks
// -----------------------------------------------------------------------
struct CardOp {
    std::function<void()> work;
    CARDCallback callback;
    s32 chan;
    s32 result;
};

static std::thread             s_worker;
static std::mutex              s_qMtx;
static std::condition_variable s_qCv;
static std::queue<CardOp>      s_opQueue;
static bool                    s_quit = false;

static void WorkerThread() {
    while (true) {
        CardOp op;
        {
            std::unique_lock<std::mutex> lk(s_qMtx);
            s_qCv.wait(lk, [] { return !s_opQueue.empty() || s_quit; });
            if (s_quit && s_opQueue.empty()) return;
            op = std::move(s_opQueue.front());
            s_opQueue.pop();
        }
        if (op.work) op.work();
        if (op.callback) op.callback(op.chan, op.result);
    }
}

static void Dispatch(s32 chan, s32 result, CARDCallback cb) {
    if (!cb) return;
    {
        std::lock_guard<std::mutex> lk(s_qMtx);
        s_opQueue.push({ nullptr, cb, chan, result });
    }
    s_qCv.notify_one();
}

// -----------------------------------------------------------------------
// Stub variables required by card.h
// -----------------------------------------------------------------------
extern "C" u32 __CARDFreq = 4; // EXI_FREQ_16M placeholder

// -----------------------------------------------------------------------
// Public API implementation
// -----------------------------------------------------------------------
extern "C" {

void CARDInit(void) {
    fs::create_directories("saves");
    s_cards[0].path = "saves/card_A.bin";
    s_cards[1].path = "saves/card_B.bin";
    s_quit = false;
    s_worker = std::thread(WorkerThread);
}

s32 CARDProbeEx(s32 chan, s32* memSize, s32* sectorSize) {
    if (chan < 0 || chan > 1) return CARD_RESULT_NOCARD;
    if (memSize)    *memSize    = 59;
    if (sectorSize) *sectorSize = CARD_SEG_SIZE;
    return CARD_RESULT_READY;
}

int CARDProbe(s32 chan) {
    return (chan == 0 || chan == 1) ? 1 : 0;
}

s32 CARDMount(s32 chan, void* /*workArea*/, CARDCallback /*detachCb*/) {
    if (chan < 0 || chan > 1) return CARD_RESULT_NOCARD;
    std::lock_guard<std::mutex> lk(s_cardMtx);
    CardState& c = s_cards[chan];
    if (!c.mounted) { LoadCard(c); c.mounted = true; }
    return CARD_RESULT_READY;
}

s32 CARDMountAsync(s32 chan, void* workArea, CARDCallback detachCb, CARDCallback attachCb) {
    s32 r = CARDMount(chan, workArea, detachCb);
    Dispatch(chan, r, attachCb);
    return r;
}

s32 CARDUnmount(s32 chan) {
    if (chan < 0 || chan > 1) return CARD_RESULT_NOCARD;
    std::lock_guard<std::mutex> lk(s_cardMtx);
    CardState& c = s_cards[chan];
    if (c.mounted && c.dirty) SaveCard(c);
    c.mounted = false;
    return CARD_RESULT_READY;
}

s32 CARDOpen(s32 chan, const char* fileName, CARDFileInfo* info) {
    if (!fileName || !info || chan < 0 || chan > 1) return CARD_RESULT_FATAL_ERROR;
    std::lock_guard<std::mutex> lk(s_cardMtx);
    CardState& c = s_cards[chan];
    if (!c.mounted) return CARD_RESULT_NOCARD;
    auto it = c.nameToNo.find(fileName);
    if (it == c.nameToNo.end()) return CARD_RESULT_NOFILE;
    info->chan   = chan;
    info->fileNo = it->second;
    info->offset = 0;
    info->length = (s32)c.data[it->second].size();
    return CARD_RESULT_READY;
}

s32 CARDFastOpen(s32 chan, s32 fileNo, CARDFileInfo* info) {
    if (!info || chan < 0 || chan > 1) return CARD_RESULT_FATAL_ERROR;
    std::lock_guard<std::mutex> lk(s_cardMtx);
    CardState& c = s_cards[chan];
    if (!c.mounted) return CARD_RESULT_NOCARD;
    if (fileNo < 0 || fileNo >= (s32)c.names.size()) return CARD_RESULT_NOFILE;
    info->chan   = chan;
    info->fileNo = fileNo;
    info->offset = 0;
    info->length = (s32)c.data[fileNo].size();
    return CARD_RESULT_READY;
}

s32 CARDClose(CARDFileInfo* /*info*/) { return CARD_RESULT_READY; }

s32 CARDCreate(s32 chan, const char* fileName, u32 size, CARDFileInfo* info) {
    if (!fileName || !info || chan < 0 || chan > 1) return CARD_RESULT_FATAL_ERROR;
    std::lock_guard<std::mutex> lk(s_cardMtx);
    CardState& c = s_cards[chan];
    if (!c.mounted) return CARD_RESULT_NOCARD;
    if (c.nameToNo.count(fileName)) return CARD_RESULT_EXIST;
    int no = (int)c.names.size();
    c.nameToNo[fileName] = no;
    c.names.emplace_back(fileName);
    c.data.emplace_back(size, (uint8_t)0);
    c.dirty = true;
    SaveCard(c);
    info->chan   = chan;
    info->fileNo = no;
    info->offset = 0;
    info->length = (s32)size;
    return CARD_RESULT_READY;
}

s32 CARDCreateAsync(s32 chan, const char* fileName, u32 size,
                    CARDFileInfo* info, CARDCallback cb) {
    s32 r = CARDCreate(chan, fileName, size, info);
    Dispatch(chan, r, cb);
    return r;
}

s32 CARDDelete(s32 chan, const char* fileName) {
    if (!fileName || chan < 0 || chan > 1) return CARD_RESULT_FATAL_ERROR;
    std::lock_guard<std::mutex> lk(s_cardMtx);
    CardState& c = s_cards[chan];
    if (!c.mounted) return CARD_RESULT_NOCARD;
    auto it = c.nameToNo.find(fileName);
    if (it == c.nameToNo.end()) return CARD_RESULT_NOFILE;
    int no = it->second;
    // Mark as deleted by clearing name (don't reindex to keep fileNo stable)
    c.names[no] = "";
    c.data[no].clear();
    c.nameToNo.erase(it);
    c.dirty = true;
    SaveCard(c);
    return CARD_RESULT_READY;
}

s32 CARDDeleteAsync(s32 chan, const char* fileName, CARDCallback cb) {
    s32 r = CARDDelete(chan, fileName);
    Dispatch(chan, r, cb);
    return r;
}

s32 CARDFastDelete(s32 chan, s32 fileNo) {
    if (chan < 0 || chan > 1) return CARD_RESULT_FATAL_ERROR;
    std::lock_guard<std::mutex> lk(s_cardMtx);
    CardState& c = s_cards[chan];
    if (!c.mounted) return CARD_RESULT_NOCARD;
    if (fileNo < 0 || fileNo >= (s32)c.names.size()) return CARD_RESULT_NOFILE;
    c.nameToNo.erase(c.names[fileNo]);
    c.names[fileNo] = "";
    c.data[fileNo].clear();
    c.dirty = true;
    SaveCard(c);
    return CARD_RESULT_READY;
}

s32 CARDFastDeleteAsync(s32 chan, s32 fileNo, CARDCallback cb) {
    s32 r = CARDFastDelete(chan, fileNo);
    Dispatch(chan, r, cb);
    return r;
}

s32 CARDRead(CARDFileInfo* info, void* buf, s32 length, s32 offset) {
    if (!info || !buf) return CARD_RESULT_FATAL_ERROR;
    std::lock_guard<std::mutex> lk(s_cardMtx);
    CardState& c = s_cards[info->chan];
    if (info->fileNo < 0 || info->fileNo >= (s32)c.data.size()) return CARD_RESULT_NOFILE;
    auto& d = c.data[info->fileNo];
    if (offset < 0 || (size_t)offset >= d.size()) return CARD_RESULT_READY;
    s32 avail = (s32)d.size() - offset;
    s32 copy  = (length < avail) ? length : avail;
    memcpy(buf, d.data() + offset, copy);
    return CARD_RESULT_READY;
}

s32 CARDReadAsync(CARDFileInfo* info, void* buf, s32 length, s32 offset, CARDCallback cb) {
    s32 r = CARDRead(info, buf, length, offset);
    Dispatch(info ? info->chan : 0, r, cb);
    return r;
}

s32 CARDWrite(CARDFileInfo* info, void* buf, s32 length, s32 offset) {
    if (!info || !buf) return CARD_RESULT_FATAL_ERROR;
    std::lock_guard<std::mutex> lk(s_cardMtx);
    CardState& c = s_cards[info->chan];
    if (info->fileNo < 0 || info->fileNo >= (s32)c.data.size()) return CARD_RESULT_NOFILE;
    auto& d = c.data[info->fileNo];
    size_t end = (size_t)offset + (size_t)length;
    if (end > d.size()) d.resize(end, 0);
    memcpy(d.data() + offset, buf, length);
    c.dirty = true;
    SaveCard(c);
    return CARD_RESULT_READY;
}

s32 CARDWriteAsync(CARDFileInfo* info, void* buf, s32 length, s32 offset, CARDCallback cb) {
    s32 r = CARDWrite(info, buf, length, offset);
    Dispatch(info ? info->chan : 0, r, cb);
    return r;
}

s32 CARDGetStatus(s32 chan, s32 fileNo, CARDStat* stat) {
    if (!stat || chan < 0 || chan > 1) return CARD_RESULT_FATAL_ERROR;
    std::lock_guard<std::mutex> lk(s_cardMtx);
    CardState& c = s_cards[chan];
    if (fileNo < 0 || fileNo >= (s32)c.names.size()) return CARD_RESULT_NOFILE;
    memset(stat, 0, sizeof(*stat));
    strncpy(stat->fileName, c.names[fileNo].c_str(), CARD_FILENAME_MAX - 1);
    stat->length = (u32)c.data[fileNo].size();
    return CARD_RESULT_READY;
}

s32 CARDSetStatus(s32 chan, s32 fileNo, CARDStat* stat) {
    (void)chan; (void)fileNo; (void)stat;
    return CARD_RESULT_READY;
}

s32 CARDSetStatusAsync(s32 chan, s32 fileNo, CARDStat* stat, CARDCallback cb) {
    s32 r = CARDSetStatus(chan, fileNo, stat);
    Dispatch(chan, r, cb);
    return r;
}

s32 CARDGetResultCode(s32 /*chan*/) { return CARD_RESULT_READY; }
s32 CARDCancel(CARDFileInfo* /*info*/) { return CARD_RESULT_READY; }
s32 CARDFormat(s32 chan) {
    if (chan < 0 || chan > 1) return CARD_RESULT_NOCARD;
    std::lock_guard<std::mutex> lk(s_cardMtx);
    CardState& c = s_cards[chan];
    c.names.clear(); c.nameToNo.clear(); c.data.clear();
    c.dirty = true;
    SaveCard(c);
    return CARD_RESULT_READY;
}

s32  CARDFreeBlocks(s32 /*chan*/, s32* byteNotUsed, s32* filesNotUsed) {
    if (byteNotUsed)  *byteNotUsed  = 59 * CARD_SEG_SIZE;
    if (filesNotUsed) *filesNotUsed = CARD_MAX_FILE;
    return CARD_RESULT_READY;
}
s32  CARDGetEncoding(s32 /*chan*/, u16* encode) { if (encode) *encode = CARD_ENCODE_ANSI; return CARD_RESULT_READY; }
s32  CARDGetMemSize(s32 /*chan*/, u16* size) { if (size) *size = 59; return CARD_RESULT_READY; }
s32  CARDGetSectorSize(s32 /*chan*/, u32* size) { if (size) *size = CARD_SEG_SIZE; return CARD_RESULT_READY; }
const DVDDiskID* CARDGetDiskID(s32 /*chan*/) { return nullptr; }
s32  CARDSetDiskID(s32 /*chan*/, const DVDDiskID* /*id*/) { return CARD_RESULT_READY; }
BOOL CARDSetFastMode(BOOL /*enable*/) { return TRUE; }
BOOL CARDGetFastMode(void) { return TRUE; }
s32  CARDGetCurrentMode(s32 /*chan*/, u32* mode) { if (mode) *mode = 0; return CARD_RESULT_READY; }

s32 CARDCheckExAsync(s32 chan, s32* xferBytes, CARDCallback cb) {
    if (xferBytes) *xferBytes = 0;
    Dispatch(chan, CARD_RESULT_READY, cb);
    return CARD_RESULT_READY;
}
s32 CARDCheckAsync(s32 chan, CARDCallback cb) {
    Dispatch(chan, CARD_RESULT_READY, cb);
    return CARD_RESULT_READY;
}
s32 CARDCheckEx(s32 /*chan*/, s32* xferBytes) { if (xferBytes) *xferBytes = 0; return CARD_RESULT_READY; }
s32 CARDCheck(s32 /*chan*/) { return CARD_RESULT_READY; }

s32 CARDErase(CARDFileInfo* /*info*/, s32 /*length*/, s32 /*offset*/) { return CARD_RESULT_READY; }
s32 CARDEraseAsync(CARDFileInfo* info, s32 length, s32 offset, CARDCallback cb) {
    s32 r = CARDErase(info, length, offset);
    Dispatch(info ? info->chan : 0, r, cb);
    return r;
}

s32 CARDProgram(CARDFileInfo* info, void* buf, s32 length, s32 offset) {
    return CARDWrite(info, buf, length, offset);
}
s32 CARDProgramAsync(CARDFileInfo* info, void* buf, s32 length, s32 offset, CARDCallback cb) {
    return CARDWriteAsync(info, buf, length, offset, cb);
}

s32  CARDRename(s32 /*chan*/, const char* /*old*/, const char* /*new_*/) { return CARD_RESULT_READY; }
s32  CARDRenameAsync(s32 chan, const char* o, const char* n, CARDCallback cb) {
    s32 r = CARDRename(chan, o, n);
    Dispatch(chan, r, cb);
    return r;
}

u16  CARDSetVendorID(u16 id) { return id; }
u16  CARDGetVendorID(void) { return 0; }
s32  CARDGetSerialNo(s32 /*chan*/, u64* serial) { if (serial) *serial = 0; return CARD_RESULT_READY; }
s32  CARDGetUniqueCode(s32 /*chan*/, u64* code) { if (code) *code = 0; return CARD_RESULT_READY; }
s32  CARDGetAttributes(s32 /*chan*/, s32 /*fileNo*/, u8* attr) { if (attr) *attr = 0; return CARD_RESULT_READY; }
s32  CARDSetAttributes(s32 /*chan*/, s32 /*fileNo*/, u8 /*attr*/) { return CARD_RESULT_READY; }
s32  CARDSetAttributesAsync(s32 chan, s32 /*fileNo*/, u8 /*attr*/, CARDCallback cb) {
    Dispatch(chan, CARD_RESULT_READY, cb);
    return CARD_RESULT_READY;
}
s32  CARDGetXferredBytes(s32 /*chan*/) { return 0; }

void CARDShutdown(void) {
    {
        std::lock_guard<std::mutex> lk(s_qMtx);
        s_quit = true;
    }
    s_qCv.notify_one();
    if (s_worker.joinable()) s_worker.join();
    std::lock_guard<std::mutex> lk(s_cardMtx);
    for (auto& c : s_cards)
        if (c.mounted && c.dirty) SaveCard(c);
}

} // extern "C"
