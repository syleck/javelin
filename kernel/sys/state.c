#include "state.h"
#include "../module.h"

MODULE("STATE");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

enum system_state system_state = SYSTEM_NORMAL;
uint64_t sysinfo[512];

void init_info(void* ad) {

}

void set_info(uint32_t a, uint64_t v) {
    if(sysinfo == 0)
        init_info(0);
    sysinfo[a] = v;
}

uint64_t get_info(uint32_t a) {
    if(sysinfo == 0)
        init_info(0);
    return sysinfo[a];
}

void trigger_update() {
    // call all functions which need an update because sysinfo may have been changed dramatically
    fbupdate();
}

void set_state(enum system_state ns) {
    mprintf("system_state = %i\n",ns);
    system_state = ns;
}

enum system_state get_state() {
    return system_state;
}