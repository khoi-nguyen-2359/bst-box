#ifndef _BSTBOX_UTILS_H_
#define _BSTBOX_UTILS_H_

char* bstbox_to_string(int value);

static inline int bstbox_is_numeric(char c) {
    return (c >= '0' && c <= '9') || c == '-';
}

static inline int bstbox_max(int a, int b) {
    return a > b ? a : b;
}

static inline int bstbox_min(int a, int b) {
    return a < b ? a : b;
}

#endif