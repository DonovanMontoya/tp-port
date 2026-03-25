#pragma once
#include "port/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef s64 OSTime;
typedef u32 OSTick;

// GC bus clock = 162 MHz, time base = bus/4 = ~40.5 MHz
// We emulate this frequency so existing time calculations stay correct.
#define OS_CORE_CLOCK 486000000u
#define OS_BUS_CLOCK  162000000u
#define OS_TIME_SPEED (OS_BUS_CLOCK / 4)  // ~40.5 MHz

#define OS_TICKS_TO_SEC(x)   ((x) / (OS_TIME_SPEED))
#define OS_TICKS_TO_MSEC(x)  ((x) / (OS_TIME_SPEED / 1000))
#define OS_TICKS_TO_USEC(x)  (((x)*8) / (OS_TIME_SPEED / 125000))
#define OS_TICKS_TO_NSEC(x)  (((x)*8000) / (OS_TIME_SPEED / 125000))

#define OS_SEC_TO_TICKS(x)   ((x) * (OS_TIME_SPEED))
#define OS_MSEC_TO_TICKS(x)  ((x) * (OS_TIME_SPEED / 1000))
#define OS_USEC_TO_TICKS(x)  ((x) * (OS_TIME_SPEED / 125000) / 8)
#define OS_NSEC_TO_TICKS(x)  ((x) * (OS_TIME_SPEED / 125000) / 8000)

OSTime OSGetTime(void);
OSTick OSGetTick(void);

#ifdef __cplusplus
}
#endif
