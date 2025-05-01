#include "L.h"

#include <stdarg.h>
#include <stdio.h>

inline FILE* logger_open() {
    return fopen("bstbox.log", "a");
}

void logger_printf(const char* format, ...) {
#ifdef LOGGABLE
    va_list args;
    va_start(args, format);
    static FILE* log_file = logger_open();
    if (!log_file) {
        log_file = logger_open();
        if (!log_file) {
            return;
        }
    }

    vfprintf(log_file, format, args);
    fflush(log_file);

    va_end(args);
#endif
}