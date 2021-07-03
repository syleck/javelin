#include "udriverapi.h"
#include "../sys/state.h"
#include "../x86/boot/grub.h"

#define HITDRIVERS_COUNT 255

struct {
	void (*hit)(int capability, int subcapability, char* module, va_list vargs);
	int capability;
} hitdrivers[HITDRIVERS_COUNT];
int hitdriverc = 0;

void udriver_hit(int capability, int subcapability, char* module,...) {
	va_list args;
	va_start(args,module);
	int state = get_state();
	set_state(UDEV_CODE);
	for(int i = 0; i < HITDRIVERS_COUNT; i++) {
		if(hitdrivers[i].capability == capability) {
			hitdrivers[i].hit(capability,subcapability,module,args);
		}
	}
	set_state(state);
	va_end(args);
}

void udriver_addc(int capability, void* fp) {
	printf("New capability handler to %i, at %x\n",capability,fp);
	hitdrivers[hitdriverc].hit = fp;
	hitdrivers[hitdriverc].capability = capability;
}

void udriver_initd(void (*fp)()) {
	int state = get_state();
	set_state(UDEV_INIT);
	fp();
	set_state(state);
}

void udriver_init() {
}