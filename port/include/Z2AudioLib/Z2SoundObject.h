#pragma once

#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/JUTAssert.h"
#ifndef JUT_ASSERT
#define JUT_ASSERT(LINE, COND) do { (void)sizeof(COND); } while (0)
#endif
#include "../../../include/Z2AudioLib/Z2SoundObject.h"
