#pragma once
/**
 * port/include/JSystem/JKernel/JKRAramArchive.h
 * PC port stub — ARAM-backed archive (GameCube/Wii ARAM not available on PC).
 * Only referenced inside #if DEBUG blocks; forward declaration is sufficient.
 */
#ifndef JKRARAMARCHIVE_H
#define JKRARAMARCHIVE_H

#include "JSystem/JKernel/JKRArchive.h"

class JKRAramArchive {
public:
    JKRAramArchive() {}
    bool mountFixed(s32 /*entryNum*/, JKRArchive::EMountDirection /*mountDirection*/) { return false; }
    u32 readResource(void*, u32, const char*) { return 0; }
    void removeResourceAll() {}
    void* mBlock = nullptr;
    void* mDvdFile = nullptr;
};

#endif /* JKRARAMARCHIVE_H */
