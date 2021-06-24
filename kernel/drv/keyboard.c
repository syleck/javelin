#include "keyboard.h"
#include "../x86/int/irq.h"
#include "../x86/asm.h"
#include "device.h"
#include "../module.h"
#include "../io/stdio.h"
#include <stdbool.h>

#define PS2_DATA 0x60
#define PS2_STAT 0x64
#define PS2_CMMD PS2_STAT

MODULE("KEYBOARD")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");
uint8_t *keycache = 0;
uint16_t key_loc = 0;

enum KEYCODE {
	NULL_KEY = 0,
	Q_PRESSED = 0x10,
	Q_RELEASED = 0x90,
	W_PRESSED = 0x11,
	W_RELEASED = 0x91,
	E_PRESSED = 0x12,
	E_RELEASED = 0x92,
	R_PRESSED = 0x13,
	R_RELEASED = 0x93,
	T_PRESSED = 0x14,
	T_RELEASED = 0x94,
	Y_PRESSED = 0x15,
	Y_RELEASED = 0x95,
	U_PRESSED = 0x16,
	U_RELEASED = 0x96,
	I_PRESSED = 0x17,
	I_RELEASED = 0x97,
	O_PRESSED = 0x18,
	O_RELEASED = 0x98,
	P_PRESSED = 0x19,
	P_RELEASED = 0x99,
	A_PRESSED = 0x1E,
	A_RELEASED = 0x9E,
	S_PRESSED = 0x1F,
	S_RELEASED = 0x9F,
	D_PRESSED = 0x20,
	D_RELEASED = 0xA0,
	F_PRESSED = 0x21,
	F_RELEASED = 0xA1,
	G_PRESSED = 0x22,
	G_RELEASED = 0xA2,
	H_PRESSED = 0x23,
	H_RELEASED = 0xA3,
	J_PRESSED = 0x24,
	J_RELEASED = 0xA4,
	K_PRESSED = 0x25,
	K_RELEASED = 0xA5,
	L_PRESSED = 0x26,
	L_RELEASED = 0xA6,
	Z_PRESSED = 0x2C,
	Z_RELEASED = 0xAC,
	X_PRESSED = 0x2D,
	X_RELEASED = 0xAD,
	C_PRESSED = 0x2E,
	C_RELEASED = 0xAE,
	V_PRESSED = 0x2F,
	V_RELEASED = 0xAF,
	B_PRESSED = 0x30,
	B_RELEASED = 0xB0,
	N_PRESSED = 0x31,
	N_RELEASED = 0xB1,
	M_PRESSED = 0x32,
	M_RELEASED = 0xB2,

	ZERO_PRESSED = 0x29,
	ONE_PRESSED = 0x2,
	NINE_PRESSED = 0xA,

	POINT_PRESSED = 0x34,
	POINT_RELEASED = 0xB4,

	SLASH_RELEASED = 0xB5,

	BACKSPACE_PRESSED = 0xE,
	BACKSPACE_RELEASED = 0x8E,
	SPACE_PRESSED = 0x39,
	SPACE_RELEASED = 0xB9,
	ENTER_PRESSED = 0x1C,
	ENTER_RELEASED = 0x9C,
};

static char* _kbrow0 = "123456789";
static char* _kbrow1 = "qwertyuiop"; // 0x10-0x1c
static char* _kbrow2 = "asdfghjkl";
static char* _kbrow3 = "zxcvbnm";
uint8_t keyboard_to_ascii(uint8_t key)
{
	if(key == BACKSPACE_PRESSED)
		return 0xff;
	if(key == 0x1C) return '\n';
	if(key == 0x39) return ' ';
	if(key == 0xE) return '\r';
	if(key == POINT_RELEASED) return '.';
	if(key == SLASH_RELEASED) return '/';
	if(key == ZERO_PRESSED) return '0';
	if(key >= ONE_PRESSED && key <= NINE_PRESSED)
		return _kbrow0[key - ONE_PRESSED];
	if(key >= 0x10 && key <= 0x1C)
	{
		return _kbrow1[key - 0x10];
	} else if(key >= 0x1E && key <= 0x26)
	{
		return _kbrow2[key - 0x1E];
	} else if(key >= 0x2C && key <= 0x32)
	{
		return _kbrow3[key - 0x2C];
	}
	return 0;
}

char lbin = 0;
bool moddn = false;
bool shiftdn = false;
bool releasing = false;

char c = 0;
char get_key() {
	c = 0;
	while(key_loc == 0);
	c = *keycache;
	key_loc--;
	for(int i = 0; i < 256; i++) {
		keycache[i] = keycache[i+1];
	}
	return c;
}

bool is_ascii(char c) {
	if(c < 126 && c > 31)
		return true;
	else
		return false;
}

int read_string(char* s, int l) {
	int rd = 0;
	int lo = l;
	bool d = false;
	while(l != 0 || !d) {
		char c = get_key();
		if(c == '\n') {
			l = 0;
			d = true;
			continue;
		} else if(c == 0xff) {
			rd--; l++;
			s[rd] = '\0';
			continue;	
		} else if(!c) {
			continue;
		}
		if(!is_ascii(c))
			continue;
		s[rd] = c;
		l--; rd++;
	}
	s[lo-1] = '\0';
}

io_struct kbstruct = {
    .read_byte = get_key,
	.read_stream = read_string,
};

void kb_int_handler(int irq) {
	keycache[key_loc++] = keyboard_to_ascii(inb(PS2_DATA));
}

void wait_cmdready() {
    while(inb(PS2_STAT)&(1<<1));
}
void wait_datready() {
    while(inb(PS2_STAT)&(1<<0));
}

void init_keyboard() {
	keycache = (uint8_t*)kmalloc(256,"KeyCache");
    set_irq(kb_int_handler,1);
    add_simple_text("kbd",kbstruct);

    outb(PS2_CMMD,0xAD); wait_cmdready();
    outb(PS2_CMMD,0xA7); wait_cmdready();
    inb(PS2_DATA);
    outb(PS2_CMMD,0x20); wait_cmdready();
    char config_byte = inb(PS2_DATA);
    mprintf("Config status byte: %x\n",config_byte);
    bool dual_channel = config_byte & 1<<5;
    if(dual_channel)
        mputs("Controller is dual channel (Most likely a mouse connected to second PS/2 port)\n");
    config_byte &= ~(1UL<<0);
    config_byte &= ~(1UL<<1);
    //config_byte &= ~(1UL<<6); 
    outb(PS2_CMMD,0x60);        wait_cmdready();
    outb(PS2_DATA,config_byte); wait_cmdready();
    mprintf("Config status byte: %x\n",config_byte);

    outb(PS2_CMMD,0xAA); wait_cmdready();
    char test_status = inb(PS2_DATA);
    if(test_status == 0x55) {
        mprintf("Controller test passed (%x)\n",test_status);
    } else {
        mprintf("Controller test failed (%x)\n",test_status);
    }
    // just incase sending OxAA resets the controller, we resend the configuration byte
    outb(PS2_CMMD,0x20);        wait_cmdready();
    char config_resendbyte = inb(PS2_DATA);
    if(config_resendbyte != config_byte) {
        mputs("Sending 0xAA reset the controller, resending configuration byte\n");
        outb(PS2_CMMD,0x60);        wait_cmdready();
        outb(PS2_DATA,config_byte); wait_cmdready();
    }
    outb(PS2_CMMD,0xAB); wait_cmdready();
    test_status = inb(PS2_DATA);
    if(!test_status) {
        mprintf("PS/2 test passed (%x)\n",test_status);
    } else {
        mprintf("PS/2 test failed (%x)\n",test_status);
    }
    config_byte |= (1UL<<0);
    outb(PS2_CMMD,0x60);        wait_cmdready();
    outb(PS2_DATA,config_byte); wait_cmdready();
    mprintf("Config status byte: %x\n",config_byte);
    outb(PS2_CMMD,0xAE); wait_cmdready();
}
