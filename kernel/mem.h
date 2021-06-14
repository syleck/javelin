#ifndef MEM_C
#define MEM_C

#include <stddef.h>
#include "module.h"

void* kmalloc(size_t size, char* module);
/**
 * @brief Wrapper for kmalloc. Automatically manages __MODULE_NAME.
 * 
 */
#define malloc(size) kmalloc(size,__MODULE_NAME);

#endif