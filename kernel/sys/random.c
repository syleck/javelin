#include "random.h"
#include "../io/stdio.h"
#include "../pci/pci.h" // use stupidint32
#include "../drv/device.h"
#include "../module.h"

stupidint32 last_read;
uint32_t last_num;

MODULE("RANDOM")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

inline uint32_t rdrand_gen() {
    uint32_t v = 0;
    __asm__ volatile("       \n"
    "    mov 100, %%ecx      \n"
    "retry:                  \n"
    "    rdrand %%eax        \n"
    "    jc .done            \n"
    "    loop retry          \n"
    ".fail:                  \n"
    "    mov 0, %0           \n"
    ".done:                  \n"
    "    mov %%eax, %0       \n"
     : "=r"(v) : : "ecx", "eax", "cc");
    last_read.l = v;
    return v;
}

char read_random_byte() {
    uint8_t ret = 0;
    if(last_num == 4) {
        last_read.l = rdrand_gen();
        last_num = 0;
    }
    switch(last_num) {
        case 0:
            ret = last_read.b.l;
            break;
        case 1:
            ret = last_read.b.l1;
            break;
        case 2:
            ret = last_read.b.h;
            break;
        case 3:
            ret = last_read.b.h1;
            break;
        default:
            break;
    }
    last_num++;
    return ret;
}

io_struct random = {
    .read_byte = read_random_byte
};

io_struct urandom = {

};

void init_random() {
    add_simple_text("x86random",random);
    add_simple_text("psurandom",urandom);
    
}