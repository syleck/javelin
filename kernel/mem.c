#include "mem.h"
#include <stdbool.h>
#include "sys/state.h"
#include "module.h"

#define MEMORY_START get_info(SYSINFO_MALLOC_START)

MODULE("MALLOC");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

struct {
    bool allocated;
    void* start;
    char* module;
    long length;
} memory_allocation_table[8192];

void* is_mem_alloc(void* address) {
    for(int i = 0; i < 8192; i++) {
        if(memory_allocation_table[i].allocated == true) {

        }
    }
}

void dbg_show_malloc_count() {
    for(int i = 0; i < 8192; i++) {
        if(memory_allocation_table[i].allocated) {
            mprintf("%s Memory allocation from %x to %x (size, %i)\n",memory_allocation_table[i].module,memory_allocation_table[i].start,memory_allocation_table[i].start+memory_allocation_table[i].length,memory_allocation_table[i].length);
        }
    }
}

int get_id_from_alloc(void* address) {
    for(int i = 0; i < 8192; i++) {
        if(memory_allocation_table[i].start == address)
            return i;
    }
}

/**
 * @brief Allocates a region of memory. Used internally because mem.h defines malloc(size) and automatically manages modules.
 * 
 * @param size Size (in bytes) of data to allocate.
 * @param module Name of module.
 * @return void* Address of memory allocated.
 */
void* kmalloc(size_t size, char* module) {
    void* address = MEMORY_START;
    for(int i = 0; i < 8192; i++) {
        if(memory_allocation_table[i].allocated == true) {
            address += memory_allocation_table[i].length;
        } else {
            void* regionalready = is_mem_alloc(address);
            if(regionalready) {
                address += memory_allocation_table[get_id_from_alloc(regionalready)].length;
                break;
            }
            #define TRACE_ALLOC
            #ifdef TRACE_ALLOC
            mprintf("Allocation at %x with a length of %iB created by %s\n",address,size,module);
            #endif
            memory_allocation_table[i].allocated = true;
            memory_allocation_table[i].start = address;
            memory_allocation_table[i].length = size;
            memory_allocation_table[i].module = module;
            break;
        }
    }
    return address;
}

void free(void* address) {
    void* mat = is_mem_alloc(address);
    if(mat) {
        memory_allocation_table[get_id_from_alloc(mat)].allocated = false;
    }
}