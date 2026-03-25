#pragma once
/**
 * port/logging.h
 * Lightweight logging for the port layer.
 */

#include <cstdio>
#include <cstdarg>

namespace tp::log {

enum class Level { Debug, Info, Warn, Error };

inline void vprint(Level lvl, const char* fmt, va_list args) {
    const char* prefix = "";
    switch (lvl) {
        case Level::Debug: prefix = "[DBG] "; break;
        case Level::Info:  prefix = "[INF] "; break;
        case Level::Warn:  prefix = "[WRN] "; break;
        case Level::Error: prefix = "[ERR] "; break;
    }
    fputs(prefix, stderr);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
}

inline void debug(const char* fmt, ...) { va_list a; va_start(a, fmt); vprint(Level::Debug, fmt, a); va_end(a); }
inline void info (const char* fmt, ...) { va_list a; va_start(a, fmt); vprint(Level::Info,  fmt, a); va_end(a); }
inline void warn (const char* fmt, ...) { va_list a; va_start(a, fmt); vprint(Level::Warn,  fmt, a); va_end(a); }
inline void error(const char* fmt, ...) { va_list a; va_start(a, fmt); vprint(Level::Error, fmt, a); va_end(a); }

} // namespace tp::log

// GameCube OSReport family — used throughout the game source
// Map to stderr so debug output is visible during development
#ifdef __cplusplus
extern "C" {
#endif

void OSReport(const char* fmt, ...);
void OSReport_Error(const char* fmt, ...);
void OSReport_Warning(const char* fmt, ...);
void OSPanic(const char* file, int line, const char* fmt, ...);

#ifdef __cplusplus
}
#endif
