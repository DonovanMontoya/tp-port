#pragma once
/**
 * port/include/JSystem/JGadget/define.h
 * PC port stub — JGadget_outMessage debug message sink (no-op).
 */
#ifndef JGADGET_DEFINE_H
#define JGADGET_DEFINE_H

#include "port/types.h"

#ifdef __cplusplus
class JGadget_outMessage {
public:
    typedef void (*MessageFunc)(const char*, int, const char*);
    static void warning(const char*, int, const char*) {}
    JGadget_outMessage(MessageFunc, const char*, int) {}
    ~JGadget_outMessage() {}
    JGadget_outMessage& operator<<(u16)          { return *this; }
    JGadget_outMessage& operator<<(u8)           { return *this; }
    JGadget_outMessage& operator<<(const char*)  { return *this; }
    JGadget_outMessage& operator<<(char)         { return *this; }
    JGadget_outMessage& operator<<(int)          { return *this; }
    JGadget_outMessage& operator<<(unsigned int) { return *this; }
    JGadget_outMessage& operator<<(const void*)  { return *this; }
    static const int BUFFER_SIZE = 256;
};
#endif // __cplusplus

// JGadget assert/warn macros → no-ops in port
#define JGADGET_ASSERTWARN(line, COND) ((void)0)
#define JGADGET_WARN(line, COND, ...) ((void)0)
#define JGADGET_WARNMSG(line, msg) ((void)0)
#define JGADGET_WARNMSG1(line, msg, arg) ((void)0)
#define JGADGET_WARNMSG3(line, msg, arg1, arg2, arg3) ((void)0)
#define JGADGET_WARNMSG4(line, msg, arg1, arg2, arg3, arg4) ((void)0)

#endif /* JGADGET_DEFINE_H */
