#include "syscall.h"
#include <stdarg.h>
#include <stdint.h>
#include "../io/stdio.h"
#include "../module.h"
#include "../drv/udriverapi.h"
#include "state.h"

MODULE("SYSCALL")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

uint32_t sysarg[128];

int do_syscall(struct regs* r) {
    asm("cli");
    uint32_t* dp;
    int matched = 0;
    if(get_state() == UDEV_INIT || get_state() == UDEV_CODE) {
        switch(r->eax) {
            case 0xfa00: // new capability
                udriver_addc(r->ebx,r->ecx);
                matched = 1;
                break;
            default:
                break;
        }
    }
    if(matched)
        goto udev_matched;
    switch(r->eax) {
    case 0: // clearscreen
        tty_clear();
        break;
    case 1: // VPRINTF, ecx=fmt pointer, ebx=va_list
        vprintf((const char*)r->ecx,(va_list*)r->ebx);
        break;
    case 2: // set sysarg, ecx=arg, ebx=v
        sysarg[r->ecx] = r->ebx;
        break;
    case 3: // get sysarg, ecx=arg, ebx=v
        r->ebx = sysarg[r->ecx];
        break;
    case 4: // load sysargs from memory, edx=addr, ebx=length, ecx=offset
        dp = (uint32_t*)r->edx;
        for(int i = 0; i < r->ebx; i++) {
            sysarg[i+r->ecx] = *(dp+=sizeof(uint32_t));
        }
        break;
    case 5: // clear args
        for(int i = 0; i < 128; i++) {
            sysarg[i] = 0;
        }
        break;
    default:
        dump_regs(r);
        PANIC("Bad system interrupt");
        break;
    }
udev_matched:
    asm("sti");
}