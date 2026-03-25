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

// Calendar time (struct from Dolphin OS)
typedef struct OSCalendarTime {
    int sec;   // [0, 60]
    int min;   // [0, 59]
    int hour;  // [0, 23]
    int mday;  // [1, 31]
    int mon;   // [0, 11]
    int year;  // e.g. 2004
    int wday;  // [0, 6]  (0 = Sunday)
    int yday;  // [0, 365]
    int msec;  // [0, 999]
    int usec;  // [0, 999]
} OSCalendarTime;

static inline void OSTicksToCalendarTime(OSTime /*ticks*/, OSCalendarTime* td) {
    if (td) {
        td->sec = td->min = td->hour = 0;
        td->mday = 1; td->mon = 0; td->year = 2004;
        td->wday = td->yday = td->msec = td->usec = 0;
    }
}
static inline OSTime OSCalendarTimeToTicks(OSCalendarTime* /*td*/) { return 0; }

#ifdef __cplusplus
}
#endif
