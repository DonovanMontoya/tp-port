#pragma once
/**
 * port/include/JSystem/JHostIO/JORReflexible.h
 * PC port stub — JOR event/reflexible interface.
 * On PC there is no HIO channel; all virtual methods are no-ops.
 */
#ifndef JORREFLEXIBLE_H
#define JORREFLEXIBLE_H

#include "port/types.h"

class JORMContext;
class JORServer;
class JORReflexible;

// ---------------------------------------------------------------------------
// Event base types
// ---------------------------------------------------------------------------
struct JOREvent {};

struct JORPropertyEvent : JOREvent {
    enum EKind {
        EKind_HasListener = (1 << 30),
        EKind_ValueID     = (1 << 29),
        EKind_FloatValue  = (1 << 9),
    };
    u8  field_0x0[4];
    u32 type;
    u32 kind;
    char* id;
    JORReflexible* field_0x10;
    u32 field_0x14;
    union {
        u32 U32;
        u16 U16[2];
    } field_0x18;
};

struct JORGenEvent : JOREvent {};

struct JORNodeEvent : JOREvent {
    u32 field_0x0;
};

// ---------------------------------------------------------------------------
// JOREventListener / JORReflexible
// ---------------------------------------------------------------------------
class JOREventListener {
public:
    JOREventListener() {}
    virtual void listenPropertyEvent(const JORPropertyEvent*) {}
};

class JORReflexible : public JOREventListener {
public:
    JORReflexible() {}
    static JORServer* getJORServer() { return nullptr; }
    virtual void listenPropertyEvent(const JORPropertyEvent*) override {}
    virtual void listen(u32, const JOREvent*) {}
    virtual void genObjectInfo(const JORGenEvent*) {}
    virtual void genMessage(JORMContext*) = 0;
    virtual void listenNodeEvent(const JORNodeEvent*) {}
    virtual ~JORReflexible() {}
};

#endif /* JORREFLEXIBLE_H */
