#include "utils.h"
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

/**
 * @brief Read integers in input string.
 * @param input Input string.
 * @param size The expected number of integers read from the input string, could be trimmed down to the actual number.
 */
int* bstbox_read_input_ints(char* input, int *size) {
    if (*size <= 0) {
        return NULL;
    }
    int* values = (int*)malloc(*size * sizeof(int));
    if (values == NULL) {
        *size = 0;
        return NULL;
    }

    char *current = input;

    // forward to the first integer
    while (*current != '\0' && !bstbox_is_numeric(*current)) {
        ++current;
    }

    int i = 0;
    int scanned = 1;
    while (i < *size && *current != '\0' && (scanned = sscanf(current, "%d", &values[i++]))) {
        while (*current != '\0' && bstbox_is_numeric(*current)) {
            ++current;
        }
        while (*current != '\0' && !bstbox_is_numeric(*current)) {
            ++current;
        }
    }
    if (scanned == 0) {
        // the last scan has failed
        i--;
    }
    if (i == 0) {
        free(values);
        *size = 0;
        return NULL;
    }
    if (i < *size) {
        int* temp = (int*)realloc(values, i * sizeof(int));
        if (temp && temp != values) {
            values = temp;
        }
        *size = i;
    }

    return values;
}