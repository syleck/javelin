#ifndef UDRIVERAPI_H
#define UDRIVERAPI_H

#include <stdarg.h>
#include "../../userspace/udev.h"

void udriver_addc(int capability, void* fp);
void udriver_initd(void (*fp)());
void udriver_hit(int capability, int subcapability, char* module,...);
void udriver_init();

#endif