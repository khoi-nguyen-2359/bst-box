#ifndef BSTBOX_UTILS_H
#define BSTBOX_UTILS_H

inline int bstbox_max(int a, int b) {
    return a > b ? a : b;
}

void bstbox_itoa(int value, char* buffer, int base);

#endif