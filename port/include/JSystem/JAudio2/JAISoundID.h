#pragma once
/**
 * port/include/JSystem/JAudio2/JAISoundID.h
 * PC port stub — JAISoundID class (wraps a u32 sound identifier).
 */
#ifndef JAISOUNDID_H
#define JAISOUNDID_H
#include "port/types.h"

class JAISoundID {
public:
    JAISoundID()             : id_(0) {}
    JAISoundID(u32 id)       : id_(id) {}
    JAISoundID(const JAISoundID& o) : id_(o.id_) {}
    operator u32() const     { return id_; }
    JAISoundID& operator=(const JAISoundID& o) { id_ = o.id_; return *this; }
    bool operator==(const JAISoundID& o) const { return id_ == o.id_; }
    bool operator!=(const JAISoundID& o) const { return id_ != o.id_; }
    u32 id_;
};

#endif
