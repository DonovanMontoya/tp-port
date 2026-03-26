#pragma once
/**
 * port/include/m_Do/m_Do_ext.h
 * PC port stub — m_Do extension helpers.
 * Shadows the original which pulls in J3DAnimation, Z2SoundObject, etc.
 * f_op_overlap.cpp includes this header.
 */
#ifndef M_DO_M_DO_EXT_H
#define M_DO_M_DO_EXT_H

#include "port/types.h"
#include "JSystem/JKernel/JKRHeap.h"

class JKRArchive;
class JKRAssertHeap;
class JKRExpHeap;
class JKRSolidHeap;
struct ResTIMG;

namespace mDoExt {

inline JKRExpHeap* getHeap(int /*tag*/) { return nullptr; }

} // namespace mDoExt

#endif /* M_DO_M_DO_EXT_H */
