#pragma once
/**
 * port/include/JSystem/JSupport/JSUMemoryStream.h
 * PC port stub — JSUMemoryInputStream / JSUMemoryOutputStream.
 */
#ifndef JSUMEMORYSTREAM_H
#define JSUMEMORYSTREAM_H

#include "port/types.h"

class JSUMemoryInputStream {
public:
    JSUMemoryInputStream(const void* buf, u32 size) : mBuf(buf), mSize(size), mPos(0) {}
    u32  read(void* dst, u32 len);
    u32  getLength() const { return mSize; }
    const void* getBuf() const { return mBuf; }
private:
    const void* mBuf;
    u32 mSize;
    u32 mPos;
};

class JSUMemoryOutputStream {
public:
    JSUMemoryOutputStream(void* buf, u32 size) : mBuf(buf), mSize(size), mPos(0) {}
    u32 write(const void* src, u32 len);
    u32 getLength() const { return mSize; }
    void* getBuf() const { return mBuf; }
private:
    void* mBuf;
    u32   mSize;
    u32   mPos;
};

#endif /* JSUMEMORYSTREAM_H */
