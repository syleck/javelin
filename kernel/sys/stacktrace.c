#include "stacktrace.h"
#include "../module.h"

MODULE("STACKTRACE");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

struct stackframe {
  struct stackframe* ebp;
  uint32_t eip;
};

void tracestack(uint32_t frames)
{
    struct stackframe *stk;
    __asm__ ("movl %%ebp,%0" : "=r"(stk) ::);
    mputs("Stack trace:\n");
    if(frames == 0)
        frames = UINT32_MAX;
    for(unsigned int frame = 0; stk && frame < frames; ++frame)
    {
        mprintf("  0x%x (next: 0x%x)    \n", stk->eip, stk->ebp);
        if(stk->ebp == 0xb00b1e5) {
            mputs("Trace end\n");
            break;
        }
        stk = stk->ebp;
    }
}