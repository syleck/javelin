#ifndef STDLIB_H
#define STDLIB_H
#include <stddef.h>

void memset(char* d, char c, int l);
void *memcpy(void *dest, const void *src, size_t len);

#endif