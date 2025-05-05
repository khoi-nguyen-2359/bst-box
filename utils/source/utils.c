#include "utils.h"

void bstbox_itoa(int value, char* buffer, int base) {
    if (base < 2 || base > 36) {
        buffer[0] = '\0';
        return;
    }
    char* ptr = buffer;
    int sign = value < 0 ? -1 : 1;
    value *= sign;
    do {
        int digit = value % base;
        *ptr++ = (digit < 10) ? '0' + digit : 'a' + digit - 10;
        value /= base;
    } while (value);
    if (sign < 0) {
        *ptr++ = '-';
    }
    *ptr-- = '\0';
    while (buffer < ptr) {
        char temp = *buffer;
        *buffer++ = *ptr;
        *ptr-- = temp;
    }
}