#include "state.h"

enum system_state state;

void set_state(enum system_state ns) {
    state = ns;
}

enum system_state get_state() {
    return state;
}