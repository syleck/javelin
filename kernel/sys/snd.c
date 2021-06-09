#include "snd.h"
#include "../drv/adlib.h"
#include "../io/stdio.h"
#include "../io/tty.h"
#include "../module.h"
#include <stdbool.h>

MODULE("SND");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

enum  {
    ADLIB,
    SB16,
    AC97,
} soundmode;

struct saf_note {
    bool    chgvA;
    uint8_t voiceAA;
    uint8_t voiceBA;
    bool    chgvB;
    uint8_t voiceAB;
    uint8_t voiceBB;
    bool    chgvC;
    uint8_t voiceAC;
    uint8_t voiceBC;
    bool    chgvD;
    uint8_t voiceAD;
    uint8_t voiceBD;
    bool    chgvE;
    uint8_t voiceAE;
    uint8_t voiceBE;
    bool    chgvF;
    uint8_t voiceAF;
    uint8_t voiceBF;
    bool    chgvG;
    uint8_t voiceAG;
    uint8_t voiceBG;
    bool    chgvH;
    uint8_t voiceAH;
    uint8_t voiceBH;
    bool    chgvI;
    uint8_t voiceAI;
    uint8_t voiceBI;
    bool    setReg;
    uint8_t srreg;
    uint8_t srvalue;
    bool    putch;
    char    ch;
    int8_t  byteskip;
    uint8_t delay; // in ms
    uint8_t notemeta; // if 0xFF, end of SAF
};

// simple audio file
void play_saf(void* begin) {
    tty_clear();
    WREG(0xa0,0);
    WREG(0xa1,0);
    WREG(0xa2,0);
    WREG(0xa3,0);
    WREG(0xa4,0);
    WREG(0xa5,0);
    WREG(0xa6,0);
    WREG(0xa7,0);
    WREG(0xa8,0);
    WREG(0xb0,0);
    WREG(0xb1,0);
    WREG(0xb2,0);
    WREG(0xb3,0);
    WREG(0xb4,0);
    WREG(0xb5,0);
    WREG(0xb6,0);
    WREG(0xb7,0);
    WREG(0xb8,0);
    struct saf_note *note_current = begin;
    tty_clear();
    while(note_current->notemeta != 0xff) {
        sleep(note_current->delay);
        update_cursor(1,0);
        printf("                       ");
        if(note_current->chgvA) {
            update_cursor(1,0);
            tty_putch('A');
            WREG(0xa0,note_current->voiceAA);
            WREG(0xb0,note_current->voiceBA);
        }
        if(note_current->chgvB) {
            update_cursor(3,0);
            tty_putch('B');
            WREG(0xa1,note_current->voiceAB);
            WREG(0xb1,note_current->voiceBB);
        }
        if(note_current->chgvC) {
            update_cursor(5,0);
            tty_putch('C');
            WREG(0xa2,note_current->voiceAC);
            WREG(0xb2,note_current->voiceBC);
        }
        if(note_current->chgvD) {
            update_cursor(7,0);
            tty_putch('D');
            WREG(0xa3,note_current->voiceAD);
            WREG(0xb3,note_current->voiceBD);
        }
        if(note_current->chgvE) {
            update_cursor(9,0);
            tty_putch('E');
            WREG(0xa4,note_current->voiceAE);
            WREG(0xb4,note_current->voiceBE);
        }
        if(note_current->chgvF) {
            update_cursor(11,0);
            tty_putch('F');
            WREG(0xa5,note_current->voiceAF);
            WREG(0xb5,note_current->voiceBF);
        }
        if(note_current->chgvG) {
            update_cursor(13,0);
            tty_putch('G');
            WREG(0xa6,note_current->voiceAG);
            WREG(0xb6,note_current->voiceBG);
        }
        if(note_current->chgvH) {
            update_cursor(15,0);
            tty_putch('H');
            WREG(0xa7,note_current->voiceAH);   
            WREG(0xb7,note_current->voiceBH);
        }
        if(note_current->chgvI) {
            update_cursor(17,0);
            tty_putch('I');
            WREG(0xa8,note_current->voiceAI);
            WREG(0xb8,note_current->voiceBI);
        }
        update_cursor(1,1);
        if(note_current->setReg) {
            printf("regm:%x <- %x",note_current->srreg,note_current->srvalue);
            WREG(note_current->srreg,note_current->srvalue);
        } else {
            printf("                 ");
        }
        update_cursor(2,2);
        if(note_current->putch) {
            tty_putch(note_current->ch); // just for laughs
        }
        update_cursor(4,3);
        printf("n:0x%x",note_current->byteskip + note_current + sizeof(struct saf_note));
        note_current = note_current->byteskip + note_current + sizeof(struct saf_note);
        update_cursor(4,2);
        printf("d:%ims",note_current->delay);
        update_cursor(1,3);
        printf("This kernel SAF player licensed under the AGPL.");
    }
    // set them all back to normal
    printf("\n");
    WREG(0xa0,0);
    WREG(0xa1,0);
    WREG(0xa2,0);
    WREG(0xa3,0);
    WREG(0xa4,0);
    WREG(0xa5,0);
    WREG(0xa6,0);
    WREG(0xa7,0);
    WREG(0xa8,0);
    WREG(0xb0,0);
    WREG(0xb1,0);
    WREG(0xb2,0);
    WREG(0xb3,0);
    WREG(0xb4,0);
    WREG(0xb5,0);
    WREG(0xb6,0);
    WREG(0xb7,0);
    WREG(0xb8,0);
}

void init_sound() {
	if(init_adlib())
        soundmode = ADLIB;

    play_saf(0);
}