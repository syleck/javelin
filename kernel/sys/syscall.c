#include "syscall.h"
#include <stdarg.h>
#include <stdint.h>
#include "../io/stdio.h"
#include "../module.h"

MODULE("SYSCALL")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

uint32_t sysarg[128];

int do_syscall(struct regs* r) {
    asm("cli");
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
        uint32_t* smn = (uint32_t*)r->edx;
        for(int i = 0; i < r->ebx; i++) {
            sysarg[i+r->ecx] = *(smn+=sizeof(uint32_t));
        }
        break;
    default:
        dump_regs(r);
        PANIC("Bad system interrupt");
        break;
    }
    asm("sti");
}