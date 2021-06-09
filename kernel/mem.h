#ifndef MEM_C
#define MEM_C

#include <stddef.h>
#include "module.h"

void* kmalloc(size_t size, char* module);
#define malloc(size) kmalloc(size,__MODULE_NAME);

#endif