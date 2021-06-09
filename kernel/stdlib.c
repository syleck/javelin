#include "stdlib.h"
#include <stddef.h>

void memset(char* d, char c, int l) {
    for(int i = 0; i < l; i++)
        *(d + i) = c;
}

void *memcpy (void *dest, const void *src, size_t len)
{
    char *d = dest;
    const char *s = src;
    while (len--)
        *d++ = *s++;
    return dest;
}