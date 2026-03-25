#pragma once
/**
 * port/include/JSystem/JUtility/TColor.h
 * PC port stub — JUtility::TColor colour type.
 * On PC we don't have GXColor as a base; just carry four u8 channels.
 */
#ifndef TCOLOR_H
#define TCOLOR_H

#include "port/types.h"

namespace JUtility {

struct TColor {
    u8 r, g, b, a;

    TColor()                       : r(0xff), g(0xff), b(0xff), a(0xff) {}
    TColor(u8 r, u8 g, u8 b, u8 a): r(r),    g(g),    b(b),    a(a)    {}
    TColor(u32 u)                  : r((u>>24)&0xff), g((u>>16)&0xff),
                                     b((u>>8)&0xff),  a(u&0xff)         {}

    operator u32() const { return ((u32)r<<24)|((u32)g<<16)|((u32)b<<8)|a; }

    void set(u8 cr, u8 cg, u8 cb, u8 ca) { r=cr; g=cg; b=cb; a=ca; }
    void set(u32 u) { r=(u>>24)&0xff; g=(u>>16)&0xff; b=(u>>8)&0xff; a=u&0xff; }

    void setColor(TColor c) { *this = c; }
};

} // namespace JUtility

#endif /* TCOLOR_H */
