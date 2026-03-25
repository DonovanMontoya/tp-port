#pragma once
/**
 * port/include/JSystem/JHostIO/JORServer.h
 * PC port stub — JHostIO server (communicates with host PC tool via HIO).
 * On PC, HIO doesn't exist; all methods are stubs returning safe defaults.
 */
#include "JSystem/JGadget/linklist.h"
#include "JSystem/JHostIO/JORReflexible.h"
#include "JSystem/JHostIO/JORMContext.h"
#include "port/types.h"

// ---------------------------------------------------------------------------
// Forward declarations for types we won't fully implement
// ---------------------------------------------------------------------------
class JORFile;
class JORDir;
class JORHostInfo_String;
class JORHostInfo_CalendarTime;

#include "JSystem/JSupport/JSUMemoryStream.h"


// ---------------------------------------------------------------------------
// JOREventCallbackListNode
// ---------------------------------------------------------------------------
struct JOREventCallbackListNode {
    JOREventCallbackListNode(u32 lo, u32 hi, bool) : field_0xc(lo), field_0x10(hi) {}
    void JORAppend() {}
    void JORRemove() {}
    virtual int  JORAct(u32, const char*) { return 0; }
    virtual ~JOREventCallbackListNode() {}
    bool JORIsAcceptableID(u32 id) const { return field_0xc <= id && id <= field_0x10; }

    JGadget::TLinkListNode m_node;
    u32 field_0xc;
    u32 field_0x10;
};

// ---------------------------------------------------------------------------
// Standalone helper functions — all no-ops
// ---------------------------------------------------------------------------
inline u32 JORMessageBox(const char*, const char*, u32) { return 0; }
inline int JORShellExecute(const char*, const char*, const char*, const char*, int) { return 0; }
inline void JORInit()  {}

// ---------------------------------------------------------------------------
// JORServer
// ---------------------------------------------------------------------------
class JORServer {
public:
    typedef void (*EventFunc)(u32, char*);
    typedef JGadget::TLinkList<JOREventCallbackListNode, -4> CallbackLinkList;

    enum ECommand {
        ECommand_GetRootObj    = 1,
        ECommand_GenObjInfo    = 3,
        ECommand_NodeEvent     = 6,
        ECommand_PropertyEvent = 7,
        ECommand_FIO           = 9,
        ECommand_ReadResultS32 = 10,
        ECommand_ReadOrEvent   = 11,
        ECommand_DIR           = 13,
        ECommand_HostInfo      = 14,
        ECommand_ReadResultU32 = 15,
    };

    JORServer()
        : mp_rootObj(nullptr), field_0x10064(0), field_0x10068(0)
        , m_event(false), m_eventDone(true), m_eventNum(0)
        , m_eventFunc(nullptr), m_isEventCallbackListEnabled(false)
    {
        m_rootName[0] = '\0';
        m_eventName[0] = '\0';
    }

    static JORServer* create() {
        if (!instance) instance = new JORServer();
        return instance;
    }

    void receive(const char*, s32) {}

    JORMContext* attachMCTX(u32)            { return &m_context; }
    void         releaseMCTX(JORMContext*)  {}

    void appendEventCallbackListNode(JOREventCallbackListNode* p) {
        if (p) m_eventCallbackList.Push_front(p);
    }
    void removeEventCallbackListNode(JOREventCallbackListNode* p) {
        if (p) m_eventCallbackList.Remove(p);
    }

    static void defSetVal(void*, u32, s32)       {}
    static void defSetBitVal(void*, u32, u16, u16) {}

    void fio_openFile_(JSUMemoryInputStream&)    {}
    void fio_closeFile_(JSUMemoryInputStream&)   {}
    void fio_readData_(JSUMemoryInputStream&)    {}
    void fio_writeData_(JSUMemoryInputStream&)   {}
    void fio_dispatchMessage_(JSUMemoryInputStream&) {}

    void dir_findFirstFile_(JSUMemoryInputStream&, JORDir*)      {}
    void dir_findNextFile_(JSUMemoryInputStream&, JORDir*)       {}
    void dir_browseForFolder_(JSUMemoryInputStream&, JORDir*)    {}
    void readResultS32_(JSUMemoryInputStream&)                   {}
    void readOrEvent_(JSUMemoryInputStream&)                     {}
    void dir_dispatchMessage_(JSUMemoryInputStream&)             {}
    void hostinfo_dispatchMessage_(JSUMemoryInputStream&)        {}
    void hostinfo_recvString_(JSUMemoryInputStream&, JORHostInfo_String*) {}
    void hostinfo_localTime_(JSUMemoryInputStream&, JORHostInfo_CalendarTime*) {}
    void readResultU32_(JSUMemoryInputStream&)                   {}

    void sendReset() {}
    void setRootNode(const char*, JORReflexible*, u32, u32) {}
    void doneEvent() {}

    bool       getEvent()              { return m_event; }
    void       setEvent(bool v)        { m_event = v; }
    void       setEventDone(bool v)    { m_eventDone = v; }
    u32        getEventNum()           { return m_eventNum; }
    char*      getEventName()          { return m_eventName; }
    EventFunc  getEventFunc()          { return m_eventFunc; }
    void       setEventFunc(EventFunc f) { m_eventFunc = f; }
    bool       isEventCallbackListEnabled() const { return m_isEventCallbackListEnabled; }
    void       enableEventCallbackList(bool e) { m_isEventCallbackListEnabled = e; }
    CallbackLinkList* referEventCallbackList() { return &m_eventCallbackList; }

    static JORServer* getInstance() { return instance; }
    static JORServer* instance;

    JORMContext    m_context;
    JORReflexible* mp_rootObj;
    char           m_rootName[64];
    u32            field_0x10064;
    u32            field_0x10068;
    bool           m_event;
    bool           m_eventDone;
    u32            m_eventNum;
    char           m_eventName[0x1000];
    EventFunc      m_eventFunc;
    bool           m_isEventCallbackListEnabled;
    CallbackLinkList m_eventCallbackList;
};

// ---------------------------------------------------------------------------
// Inline helpers from original header
// ---------------------------------------------------------------------------
inline u32 JOR_MESSAGELOOP() {
    JORServer* server = JORServer::getInstance();
    if (!server) return 0;
    if (server->getEvent()) {
        server->setEvent(false);
        server->setEventDone(false);
        u32 eventNum = server->getEventNum();
        char* eventName = server->getEventName();
        if (server->isEventCallbackListEnabled() && eventNum >= 0x80000000u) {
            JORServer::CallbackLinkList* list = server->referEventCallbackList();
            for (auto it = list->begin(); it != list->end();) {
                JOREventCallbackListNode& cb = *it; ++it;
                if (!cb.JORIsAcceptableID(eventNum)) continue;
                if (cb.JORAct(eventNum, eventName)) break;
            }
        } else {
            JORServer::EventFunc fn = server->getEventFunc();
            if (fn) fn(eventNum, eventName);
        }
        server->doneEvent();
    }
    return 0; // no HIO event loop on PC
}

inline void JOR_INIT() {
    JORInit();
    JORServer::create()->setEventFunc(nullptr);
}
inline void JOR_SETROOTNODE(const char* name, JORReflexible* node, u32 p3, u32 p4) {
    JORServer::getInstance()->setRootNode(name, node, p3, p4);
}
inline JORMContext* attachJORMContext(u32 id)    { return JORServer::getInstance()->attachMCTX(id); }
inline JORMContext* JORAttachMContext(u32 id)    { return JORServer::getInstance()->attachMCTX(id); }
inline void releaseJORMContext(JORMContext* ctx)  { JORServer::getInstance()->releaseMCTX(ctx); }
inline void JORReleaseMContext(JORMContext* ctx)  { JORServer::getInstance()->releaseMCTX(ctx); }
inline void JOR_ENABLEEVENTCALLBACKLIST(bool en)  { JORServer::getInstance()->enableEventCallbackList(en); }
