#pragma once
/**
 * port/include/d/d_msg_object.h
 * PC port stub — message object class.
 * Shadows the original which pulls in JSystem/JMessage/control.h,
 * mDoDvdThd_mountXArchive_c, JUtility::TColor chains, J2DPane/J2DPicture.
 * f_op_msg_mng.cpp uses this header.
 */
#ifndef D_MSG_D_MSG_OBJECT_H
#define D_MSG_D_MSG_OBJECT_H

#include "port/types.h"
#include "f_op/f_op_msg.h"
#include "SSystem/SComponent/c_xyz.h"

class fopAc_ac_c;
class J2DTextBox;
class JUTFont;
class COutFont_c;
class mDoDvdThd_mountXArchive_c;

// -----------------------------------------------------------------------
// J2DPane / J2DPicture stubs — referenced in f_op_msg_mng.cpp dummy funcs
// -----------------------------------------------------------------------
class J2DPane {
public:
    virtual ~J2DPane() {}
    u8  getAlpha()       const { return 0xFF; }
    f32 getHeight()      const { return 0.0f; }
    f32 getWidth()       const { return 0.0f; }
    void setAlpha(u8)          {}
};

class J2DPicture : public J2DPane {
public:
    virtual ~J2DPicture() {}
    // Wii/Shield variant — append(const char*, f32)
    virtual void append(const char*, f32)            {}
    virtual void setBlendRatio(f32, f32)             {}
};

// -----------------------------------------------------------------------
// dMsgObject_c — minimal stub inheriting from msg_class
// The original class is ~0x250 bytes with JMessage state; on PC we only
// need the members accessed directly in f_op_msg_mng.cpp.
// -----------------------------------------------------------------------
class dMsgObject_c : public msg_class {
public:
    // Called by fopMsgM_messageSet to arm a pending message
    void setMessageIndex(u32 /*msgIdx*/, u32 /*param*/, bool /*flag*/) {}
    void setMessageIndexDemo(u32, bool) {}
    void endFlowGroup() {}
    void setTalkActor(fopAc_ac_c*) {}
    u32  getMessageIndex(u32) { return 0; }
    bool isDraw() { return false; }
    u16  getStatus() const { return 0; }
    s16  getSmellTypeMessageID() const { return 0; }
    u8   getSelectCursorPos() const { return 0; }
    bool isMouthCheck() const { return false; }
    u8   getSelectBombBagID() const { return 0; }
    void readMessageGroup(mDoDvdThd_mountXArchive_c**) {}
    void changeFlowGroup(s32) {}
    void demoMessageGroup() {}
    void setDemoMessage(u32) {}
    void offMsgSendControl() {}
    void onMsgSendControl() {}
    int  isMsgSendControl() const { return 0; }
    int  isMsgSend() const { return 0; }
    void offMsgSend() {}
    bool getString(u32, J2DTextBox*, J2DTextBox*, JUTFont*, COutFont_c*,
                   char*, char*, char*, s16*) { return false; }
    void setSmellType(u8) {}
    void setSelectCursorPos(u8) {}
    void setTalkPartner(fopAc_ac_c*) {}
    void setInsectItemNo(u8) {}
    bool isSelectTalkNowCheck() { return false; }
};

// -----------------------------------------------------------------------
// dMsgObject_HIO_c — HIO (host IO) debug config class
// Only mMsgDebug / mMsgIndex accessed in f_op_msg_mng.cpp
// -----------------------------------------------------------------------
class dMsgObject_HIO_c {
public:
    dMsgObject_HIO_c() : mMsgDebug(false), mMsgIndex(0) {}
    virtual ~dMsgObject_HIO_c() {}
    bool mMsgDebug;
    u32  mMsgIndex;
};
extern dMsgObject_HIO_c g_MsgObject_HIO_c;

// -----------------------------------------------------------------------
// Inline free-function wrappers
// All forward through dComIfGp_getMsgObjectClass() which returns nullptr on PC.
// dComIfGp_getMsgObjectClass() is defined in d_com_inf_game.h.
// -----------------------------------------------------------------------
#include "d/d_com_inf_game.h"

inline void dMsgObject_setTalkActor(fopAc_ac_c* actor) {
    if (dMsgObject_c* m = dComIfGp_getMsgObjectClass()) m->setTalkActor(actor);
}
inline void dMsgObject_endFlowGroup() {
    if (dMsgObject_c* m = dComIfGp_getMsgObjectClass()) m->endFlowGroup();
}
inline void dMsgObject_setDemoMessage(u32 idx) {
    if (dMsgObject_c* m = dComIfGp_getMsgObjectClass()) m->setDemoMessage(idx);
}
inline void dMsgObject_offMsgSendControl() {
    if (dMsgObject_c* m = dComIfGp_getMsgObjectClass()) m->offMsgSendControl();
}
inline void dMsgObject_onMsgSendControl() {
    if (dMsgObject_c* m = dComIfGp_getMsgObjectClass()) m->onMsgSendControl();
}
inline int dMsgObject_isMsgSendControl() {
    dMsgObject_c* m = dComIfGp_getMsgObjectClass();
    return m ? m->isMsgSendControl() : 0;
}
inline int dMsgObject_isMsgSend() {
    dMsgObject_c* m = dComIfGp_getMsgObjectClass();
    return m ? m->isMsgSend() : 0;
}
inline void dMsgObject_offMsgSend() {
    if (dMsgObject_c* m = dComIfGp_getMsgObjectClass()) m->offMsgSend();
}
inline bool dMsgObject_isSelectTalkNow() {
    dMsgObject_c* m = dComIfGp_getMsgObjectClass();
    return m ? m->isSelectTalkNowCheck() : false;
}
inline void dMsgObject_setSmellType(u8 t) {
    if (dMsgObject_c* m = dComIfGp_getMsgObjectClass()) m->setSmellType(t);
}
inline u8 dMsgObject_getSelectCursorPos() {
    dMsgObject_c* m = dComIfGp_getMsgObjectClass();
    return m ? m->getSelectCursorPos() : 0;
}
inline void dMsgObject_setSelectCursorPos(u8 p) {
    if (dMsgObject_c* m = dComIfGp_getMsgObjectClass()) m->setSelectCursorPos(p);
}
inline void dMsgObject_setTalkPartner(fopAc_ac_c* actor) {
    if (dMsgObject_c* m = dComIfGp_getMsgObjectClass()) m->setTalkPartner(actor);
}
inline void dMsgObject_setInsectItemNo(u8 no) {
    if (dMsgObject_c* m = dComIfGp_getMsgObjectClass()) m->setInsectItemNo(no);
}
inline void dMsgObject_readMessageGroup(mDoDvdThd_mountXArchive_c** p) {
    if (dMsgObject_c* m = dComIfGp_getMsgObjectClass()) m->readMessageGroup(p);
}
inline void dMsgObject_changeFlowGroup(s32 flow) {
    if (dMsgObject_c* m = dComIfGp_getMsgObjectClass()) m->changeFlowGroup(flow);
}
inline void dMsgObject_demoMessageGroup() {
    if (dMsgObject_c* m = dComIfGp_getMsgObjectClass()) m->demoMessageGroup();
}
inline bool dMsgObject_getString(u32 id, J2DTextBox* t1, J2DTextBox* t2, JUTFont* font,
                                  COutFont_c* out, char* text, char* ruby,
                                  char* name, s16* idx) {
    dMsgObject_c* m = dComIfGp_getMsgObjectClass();
    return m ? m->getString(id, t1, t2, font, out, text, ruby, name, idx) : false;
}
inline bool dMsgObject_isSelectTalkNowCheck() { return dMsgObject_isSelectTalkNow(); }
inline void dMsgObject_setKillMessageFlag() {}
inline void dMsgObject_setSelectWordFlag(u8) {}
inline s16  dMsgObject_getSmellTypeMessageID() {
    dMsgObject_c* m = dComIfGp_getMsgObjectClass();
    return m ? m->getSmellTypeMessageID() : 0;
}
inline u8   dMsgObject_getSelectBombBagID() {
    dMsgObject_c* m = dComIfGp_getMsgObjectClass();
    return m ? m->getSelectBombBagID() : 0;
}
inline bool dMsgObject_isMouthCheck() {
    dMsgObject_c* m = dComIfGp_getMsgObjectClass();
    return m ? m->isMouthCheck() : false;
}
inline void dMsgObject_setOffering(u16) {}

#endif /* D_MSG_D_MSG_OBJECT_H */
