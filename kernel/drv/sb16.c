#include "sb16.h"
#include "../module.h"
#include "../x86/asm.h"
#include "../mem.h"
#include "../stdlib.h"

#define SB16_MIXER_CMD 0x224
#define SB16_MIXER_DAT 0x225
#define SB16_DSP_RESET 0x226
#define SB16_DSP_READ  0x22A
#define SB16_DSP_WRITE 0x22C
#define SB16_READ_STAT 0x22E
#define SB16_16B_INTER 0x22F

#define BITS_HIGH(x) \
    (x >> 8) & 0xFF

#define BITS_LOW(x) \
    x & 0xFF

MODULE("SB16")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

int init_sb16() {
    outb(SB16_DSP_RESET,1);
    outb(SB16_DSP_RESET,0);
    char resp = inb(SB16_DSP_READ);
    if(resp = 0xAA) {
        mputs("SB16 returned 0xAA\n");
        return 1;
    } else {
        mprintf("Warn: SB16 returned %x\n", resp);
        return 0;
    }
}

void play_sound(void* addr, int size, int volume, int samplerate, int channels) {
    // i hate the ISA DMA! i hate the ISA DMA!
    outb(SB16_DSP_WRITE,0xD1);

    outb(0xA,0x5); // disable channel 1
    outb(0xC,1); // write 1 to flipflop
    outb(0xB,0x48);
    outb(0x83,0x0);
    outb(0x2,BITS_LOW((uint16_t)addr)); // set address
    outb(0x2,BITS_HIGH((uint16_t)addr));
    outb(0x3,BITS_LOW(size)); // set size
    outb(0x3,BITS_HIGH(size));
    outb(0x3,0x1); // enable channel 1
    
    //uint8_t time_constant = 65536 - (256000000 / (channels * samplerate));

    outb(SB16_DSP_WRITE,0x40);
    outb(SB16_DSP_WRITE,165);
    outb(SB16_DSP_WRITE,0xC0);
    outb(SB16_DSP_WRITE,0x00);

    mprintf("Playing sound from %x with a size %x with volume %i, samplerate %i and %i channels\n",addr,size,volume,samplerate,channels);
    outb(SB16_DSP_WRITE,BITS_LOW(size)-1);
    outb(SB16_DSP_WRITE,BITS_HIGH(size));
    __asm__("sti");    
    mputs("Interrupts enabled\n");
}