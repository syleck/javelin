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

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void* is_mem_alloc(void* address) {
    for(int i = 0; i < 8192; i++) {
        if(memory_allocation_table[i].allocated == true) {
            if(memory_allocation_table[i].start + memory_allocation_table[i].length < address)
                continue;
            if(memory_allocation_table[i].start > address)
                continue;
            return memory_allocation_table[i].start;
        }
    }
}

void dbg_show_malloc_count() {
    for(int i = 0; i < 8192; i++) {
        if(memory_allocation_table[i].allocated) {
            mprintf("%s Memory allocation from %x to %x (size, %i)\n",memory_allocation_table[i].module,memory_allocation_table[i].start,memory_allocation_table[i].start+memory_allocation_table[i].length,memory_allocation_table[i].length);
        } else {
            mputs("Unallocated entry\n");
        }
    }
}

int get_id_from_alloc(void* address) {
    static bool trying;
    for(int i = 0; i < 8192; i++) {
        if(memory_allocation_table[i].start == address)
            return i;
    }
    if(trying)
        return 0;
    trying = true;
    return get_id_from_alloc(is_mem_alloc(address));
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
            if(is_mem_alloc(address));
                address += memory_allocation_table[i].length;
        } else {
            void* regionalready = is_mem_alloc(address);
            if(regionalready) {
                address += memory_allocation_table[get_id_from_alloc(regionalready)].length;
                continue;
            }
            //#define TRACE_ALLOC
            //#ifdef TRACE_ALLOC
            //#endif
            memory_allocation_table[i].allocated = true;
            memory_allocation_table[i].start = address;
            memory_allocation_table[i].length = size;
            memory_allocation_table[i].module = module;
            break;
        }
    }
    mprintf("Allocation at %x with a length of %iB created by %s\n",address,size,module);
    return address;
}

void free(void* address) {
    void* mat = is_mem_alloc(address);
    if(mat) {
        memory_allocation_table[get_id_from_alloc(mat)].allocated = false;
    }
}

extern void loadPageDirectory(unsigned int*);
extern void enablePaging();

void meminit() {
    mputs("Starting paging\n");
    for(int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;
    }
    for(int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | 3;
    }
    page_directory[0] = ((unsigned int)first_page_table) | 3;
    loadPageDirectory(page_directory);
    mputs("Page directory loaded\n");
    enablePaging();
    mputs("Paging initialized\n");
}