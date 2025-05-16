#include "bstbox_utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* bstbox_to_string(int value) {
    const int max = 11;
    char* buffer = (char*)malloc(max + 1); // 32 bits int can be represented in 11 digits + 1 for null terminator
    if (buffer == NULL) {
        return NULL;
    }
    int actual = snprintf(buffer, max + 1, "%d", value);
    if (actual < max) {
        char* temp = (char*)realloc(buffer, actual + 1);
        if (temp && temp != buffer) {
            buffer = temp;
        }
    }

    return buffer;
}
