#include "bstbox_io.h"
#include "bstbox_utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Read integers in input string.
 * @param input Input string.
 * @param size The expected number of integers read from the input string, could be trimmed down to the actual number.
 */
int* bstbox_read_ints(char* input, int *size) {
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

/**
 * @brief Read a line from a file, dynamically allocating memory for the line.
 * @param file Input file stream.
 * @param len Pointer to store the length of the line read.
 * @return Dynamically allocated string containing the line, or NULL on failure.
 */
char* bstbox_read_line(FILE* file, size_t* len) {
    if (!file || !len) {
        return NULL;
    }

    size_t capacity = 128; // Initial buffer size
    size_t size = 0;
    char* buffer = (char*)malloc(capacity);
    if (!buffer) {
        return NULL;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (size + 1 >= capacity) {
            capacity *= 2;
            char* temp = (char*)realloc(buffer, capacity);
            if (!temp) {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }
        buffer[size++] = (char)ch;
        if (ch == '\n') {
            break;
        }
    }

    if (size == 0 && ch == EOF) {
        free(buffer);
        return NULL;
    }

    buffer[size] = '\0';
    *len = size;
    return buffer;
}
