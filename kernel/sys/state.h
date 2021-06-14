#ifndef STATE_H
#define STATE_H

enum system_state {
    SYSTEM_NORMAL,
    SYSTEM_IRQ,
    SYSTEM_ISR,
    SYSTEM_PANIC,
    SYSTEM_MONITOR,
};

void set_state(enum system_state ns);
enum system_state get_state();

#endif