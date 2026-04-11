#pragma once
/**
 * port/include/d/actor/d_a_midna.h
 * PC port stub — Midna actor class.
 * Only referenced inside #if DEBUG blocks; forward declaration is sufficient.
 */

#if TP_PORT_USE_REAL_ALINK
#include "../../../../include/d/actor/d_a_midna.h"
#else

#ifndef D_A_MIDNA_H
#define D_A_MIDNA_H

class daMidna_c {
public:
    static bool checkMidnaRealBody() { return false; }
    bool checkNoDraw() const { return true; }
};
class daMidna_hio_c1;

#endif /* D_A_MIDNA_H */

#endif
