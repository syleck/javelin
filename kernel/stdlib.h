#ifndef STDLIB_H
#define STDLIB_H
#include <stddef.h>

/**
 * @brief Sets region of memory to c
 * 
 * @param d Pointer to memory.
 * @param c Value to set.
 * @param l Length of data.
 */
void memset(char* d, char c, int l);
/**
 * @brief Copies memory from src to dest.
 * 
 * @param dest Destination
 * @param src Source
 * @param len Length of data
 * @return void* 
 */
void *memcpy(void *dest, const void *src, size_t len);

#endif