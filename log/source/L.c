#include "L.h"

#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>

static FILE* log_file = NULL;
static pthread_mutex_t log_file_mutex = PTHREAD_MUTEX_INITIALIZER;

void logger_close() {
    if (log_file) {
        fclose(log_file);
    }
}

void logger_printf(const char* format, ...) {
#ifdef LOGGABLE
    va_list args;
    va_start(args, format);

    pthread_mutex_lock(&log_file_mutex);

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
        pthread_mutex_unlock(&log_file_mutex);
        va_end(args);
        return;
#endif
}