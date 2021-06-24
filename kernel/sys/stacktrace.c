#include "stacktrace.h"
#include "../module.h"
#include "../x86/boot/grub.h"

MODULE("STACKTRACE");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

struct stackframe {
  struct stackframe* ebp;
  uint32_t eip;
};

/**
 * @brief Traces the stackframe.
 * 
 * @param frames Count of frames. If 0, frames = UINT32_MAX.
 */
void tracestack(uint32_t frames)
{
    struct stackframe *stk;
    __asm__ ("movl %%ebp,%0" : "=r"(stk) ::);
    mputs("Stack trace:\n");
    if(frames == 0)
        frames = UINT32_MAX;
    for(unsigned int frame = 0; stk && frame < frames; ++frame)
    {
        mprintf("  0x%x %s (next: 0x%x)\n", stk->eip, get_symbol(stk->eip), stk->ebp);
        if(stk->ebp == 0xb00b1e5) {
            mputs("Trace end\n");
            break;
        }
        stk = stk->ebp;
    }
}