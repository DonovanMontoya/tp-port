#pragma once
/**
 * port/include/JSystem/JUtility/JUTReport.h
 * PC port shadow — routes JUTReport output through printf.
 */
#include <cstdio>
#include <cstdarg>

inline void JUTReport(int /*x_pos*/, int /*y_pos*/, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ::vprintf(fmt, args);
    va_end(args);
    ::printf("\n");
}

inline void JUTReport(int /*x_pos*/, int /*y_pos*/, int /*unused*/, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ::vprintf(fmt, args);
    va_end(args);
    ::printf("\n");
}
