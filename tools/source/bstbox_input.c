#include "bstbox_input.h"
#include "bstbox_utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static inline int is_not_eol(char c);
static inline int is_eol(char c);

/**
 * @brief Read integers from the first line of the input string, dynamically growing the output array as needed.
 * @param input Input string.
 * @param size Pointer to store the number of integers read.
 * @return Dynamically allocated array of integers, or NULL if no integers are found.
 */
int* bstbox_read_ints(char* input, size_t *size) {
    if (!input || !size) {
        return NULL;
    }

    size_t capacity = 32; // Initial capacity
    int* values = (int*)malloc(capacity * sizeof(int));
    if (!values) {
        *size = 0;
        return NULL;
    }

    char *current = input;
    *size = 0;

    while (is_not_eol(*current)) {
        // Skip non-numeric characters
        while (is_not_eol(*current) && !bstbox_is_numeric(*current)) {
            ++current;
        }

        if (is_eol(*current)) {
            break;
        }

        // Parse the integer
        int value;
        int scanned = sscanf(current, "%d", &value);
        if (scanned == 0) {
            break;
        }

        // Grow the array if needed
        if (*size >= capacity) {
            capacity *= 2;
            int* temp = (int*)realloc(values, capacity * sizeof(int));
            if (!temp) {
                return values;
            }
            values = temp;
        }

        values[(*size)++] = value;

        // Move to the next potential integer
        while (is_not_eol(*current) && bstbox_is_numeric(*current)) {
            ++current;
        }
    }

    if (*size == 0) {
        free(values);
        return NULL;
    }

    // Shrink the array to fit the actual size
    int* temp = (int*)realloc(values, (*size) * sizeof(int));
    if (temp) {
        values = temp;
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

    int capacity = 128; // Initial buffer size
    *len = 0;
    char* buffer = (char*)malloc(capacity);
    if (!buffer) {
        return NULL;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (*len + 1 >= capacity) {
            capacity *= 2;
            char* temp = (char*)realloc(buffer, capacity);
            if (!temp) {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }
        buffer[(*len)++] = (char)ch;
        if (ch == '\n') {
            break;
        }
    }

    if (*len == 0 && ch == EOF) {
        free(buffer);
        return NULL;
    }

    buffer[*len] = '\0';
    return buffer;
}

static inline int is_eol(char c) {
    return c == '\0' || c == '\n';
}

static inline int is_not_eol(char c) {
    return !is_eol(c);
}
