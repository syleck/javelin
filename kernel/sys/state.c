#include "state.h"

enum system_state system_state = SYSTEM_NORMAL;
uint32_t *sysinfo;

void init_info(void* ad) {
    if(ad != 0) {
        sysinfo = ad;
    } else {
        sysinfo = kmalloc(512,"Sysinfo data");
    }
}

void set_info(uint32_t a, uint32_t v) {
    if(sysinfo == 0)
        init_info(0);
    sysinfo[a] = v;
}

uint32_t get_info(uint32_t a) {
    if(sysinfo == 0)
        init_info(0);
    return sysinfo[a];
}

void trigger_update() {
    // call all functions which need an update because sysinfo may have been changed dramatically
    fbupdate();
}

void set_state(enum system_state ns) {
    printf("system_state = %i\n",ns);
    system_state = ns;
}

enum system_state get_state() {
    return system_state;
}