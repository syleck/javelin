#include "adlib.h"
#include "../x86/int/irq.h"
#include "../module.h"

MODULE("ADLIB");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

io_struct adlib = {

    .cmode = IO_INPUT,
};

int init_adlib() {
    WREG(0x04,0x60);
    WREG(0x04,0x80);
    uint8_t sr0 = inb(0x388);
    WREG(0x02,0xff);
    WREG(0x04,0x21);
    sleep(80);
    uint8_t sr1 = inb(0x388);
    WREG(0x04,0x60);
    WREG(0x04,0x80);
    if((sr0 & 0xe0)==0x00 && (sr1 & 0xe0)==0xc0) {
        mputs("Adlib found & working\n");
        return 1;
    } else {
        mprintf("SR0: %x, SR1: %x. failing (not detected or malfunctioning)\n",sr0,sr1);\
        return 0;
    }
}