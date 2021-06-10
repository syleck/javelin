#include "x86/gdt.h"
#include "x86/task.h"
#include "x86/int/isr.h"
#include "x86/int/irq.h"
#include "x86/bits16/v86.h"
#include "x86/idt.h"
#include "x86/asm.h"
#include "fs/fs.h"
#include "pci/pci.h"
#include "io/tty.h"
#include "io/serial.h"
#include "io/bochs.h"
#include "io/stdio.h"
#include "sys/random.h"
#include "sys/snd.h"
#include "drv/device.h"
#include "drv/keyboard.h"
#include "drv/adlib.h"
#include "drv/sb16.h"
#include "fs/iso9660.h"
#include "module.h"

MODULE("KERNEL");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

extern int* _SOUNDTEST_START;
extern int* _SOUNDTEST_END;

void kernel_main() {
	init_tty();
	init_serial();
	init_bochs();
	printf("Javelin kernel revision 2\n");
	printf("Special flags: ");
	#ifdef PANIC_ON_OOPS
	printf("PANIC_ON_OOPS ");
	#endif
	#ifdef ONT_DO_MODULES
	printf("DONT_DO_MODULES (ONT_DO_MODULES) ");
	#endif
	printf("\n");
	gdt_install();
	idt_install();
	isr_install();
	irq_install();

	// do pit init here cause im lazy to make another file
	uint16_t divisor = (uint16_t) (1193180 / 1000); // 1000Hz will make the PIT fire IRQ #0 every 1ms
    uint8_t low = (uint8_t) (divisor & 0xFF);
    uint8_t high = (uint8_t) ((divisor >> 8) & 0xFF);
	outb(0x43, 0x36);
    outb(0x40, low);
    outb(0x40, high);
	
	init_keyboard();
	pci_install();
	//init_ata_pio();
	init_random();
	//init_sound();
	for(;;) {
		sleep(1);
	}
}
