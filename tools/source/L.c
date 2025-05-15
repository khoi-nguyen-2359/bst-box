#include "L.h"

#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>

static FILE* log_file = NULL;

void logger_close() {
#ifdef LOGGABLE
    if (log_file) {
        fclose(log_file);
    }
#endif
}

void logger_printf(const char* format, ...) {
#ifdef LOGGABLE
    va_list args;
    va_start(args, format);

    if (log_file == NULL || ferror(log_file) != 0 || fflush(log_file) != 0) {
        logger_close();
        log_file = fopen("bstbox.log", "a");
    }
    if (!log_file) {
        goto cleanup;
    }

    vfprintf(log_file, format, args);
    fflush(log_file);

    cleanup:
        va_end(args);
        return;
#endif
}