#pragma once
/**
 * port/include/JSystem/JHostIO/JORMContext.h
 * PC port stub — JOR context for HIO property messages.
 * All gen* methods are no-ops; there is no host tool on PC.
 */
#ifndef JORMCONTEXT_H
#define JORMCONTEXT_H

#include "port/types.h"
#include "JSystem/JHostIO/JORReflexible.h"

class JORMContext {
public:
    JORMContext() {}

    // ---- control generators (no-ops) ----
    void genSlider(const char*, u8*,  int, int)          {}
    void genSlider(const char*, s8*,  int, int)          {}
    void genSlider(const char*, u16*, int, int)          {}
    void genSlider(const char*, s16*, int, int)          {}
    void genSlider(const char*, u32*, int, int)          {}
    void genSlider(const char*, s32*, int, int)          {}
    void genSlider(const char*, f32*, f32, f32)          {}
    void genCheckBox(const char*, u8*, u8)               {}
    void genCheckBox(const char*, bool*, bool)           {}
    void genButton(const char*, int)                     {}
    void genLabel(const char*, int)                      {}
    void genS32(const char*, s32*)                       {}
    void genU8(const char*, u8*)                         {}
    void genF32(const char*, f32*)                       {}
    void genString(const char*, char*, u32)              {}
    void genNode(const char*, JORReflexible*)            {}
    void genStartNode(const char*)                       {}
    void genEndNode()                                    {}
};

#define MCTX_MSG_RESET          0
#define MCTX_MSG_GET_ROOT_OBJ   2
#define MCTX_MSG_GEN_OBJ_INFO   4
#define MCTX_MSG_UPDATE_NODE    8

#endif /* JORMCONTEXT_H */
