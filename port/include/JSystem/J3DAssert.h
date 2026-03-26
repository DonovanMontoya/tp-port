#pragma once

#include "JSystem/JUtility/JUTAssert.h"

#define J3D_ASSERT_NULLPTR(LINE, COND) \
    do { JUT_ASSERT_MSG(LINE, (COND) != 0, "Error : null pointer."); } while (0);
#define J3D_ASSERT_RANGE(LINE, COND) \
    do { JUT_ASSERT_MSG(LINE, (COND) != 0, "Error : range over."); } while (0);
#define J3D_ASSERT_NONZEROARG(LINE, COND) \
    do { JUT_ASSERT_MSG(LINE, (COND) != 0, "Error : non-zero argument is specified 0."); } while (0);
#define J3D_ASSERT_ALLOCMEM(LINE, COND) \
    do { JUT_ASSERT_MSG(LINE, (COND) != 0, "Error : allocate memory."); } while (0);
