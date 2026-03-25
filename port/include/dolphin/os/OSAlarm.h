#pragma once
#include "port/types.h"
#include "dolphin/os/OSTime.h"

#ifdef __cplusplus
extern "C" {
#endif

// Forward-declare OSContext (defined in OSContext.h, included before this in os.h)
typedef struct OSContext OSContext;

typedef struct OSAlarm OSAlarm;
typedef void (*OSAlarmHandler)(OSAlarm* alarm, OSContext* context);

struct OSAlarm {
    OSAlarmHandler handler;
    u32            tag;
    OSTime         fire;
    OSAlarm*       prev;
    OSAlarm*       next;
    OSTime         period;
    OSTime         start;
};

// PC stubs: all no-ops
static inline void OSCreateAlarm(OSAlarm* alarm) {
    if (alarm) {
        alarm->handler = 0;
        alarm->tag     = 0;
        alarm->fire    = 0;
        alarm->prev    = 0;
        alarm->next    = 0;
        alarm->period  = 0;
        alarm->start   = 0;
    }
}
static inline void OSSetAlarm(OSAlarm* /*alarm*/, OSTime /*tick*/, OSAlarmHandler /*handler*/) {}
static inline void OSSetPeriodicAlarm(OSAlarm* /*alarm*/, OSTime /*start*/, OSTime /*period*/, OSAlarmHandler /*handler*/) {}
static inline void OSCancelAlarm(OSAlarm* /*alarm*/) {}

#ifdef __cplusplus
}
#endif
