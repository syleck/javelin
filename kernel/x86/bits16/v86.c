#include "v86.h"
#include "../../stdlib.h"
#include "../../module.h"

MODULE("V8086");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

extern void enter_v86(uint32_t ss, uint32_t esp, uint32_t cs, uint32_t eip);

extern uint32_t* _BITS16_START_ADDR;
extern uint32_t* _BITS16_END_ADDR;

void start_v86_sub(v86_subroutine sub) {
    switch(sub) {
        default:
            mprintf("Unknown sub id: %i\n",sub);
            break;
    }
}

void init_v86() {
    uint32_t bits16_size = _BITS16_START_ADDR-_BITS16_END_ADDR;
    //memcpy(0x00007C00,_BITS16_START_ADDR,bits16_size);
    mprintf("Copied bits16 segment of size %i (%x to %x) to 7c00\n",bits16_size,_BITS16_START_ADDR,_BITS16_END_ADDR);
}