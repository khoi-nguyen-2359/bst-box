#ifndef _BSTBOX_IO_H_
#define _BSTBOX_IO_H_

#include <stdio.h>

int* bstbox_read_ints(char* input, size_t *size);
char* bstbox_read_line(FILE* file, size_t* size);

#endif
