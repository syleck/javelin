#ifndef MODULE_H
#define MODULE_H

#include "io/stdio.h"

//static char* __MODULE_NAME __attribute((unused)) = "UNKNOWN MODULE";

#ifndef ONT_DO_MODULES
#define MODULE(name) static char* __MODULE_NAME __attribute__((unused)) = name;
#define MODULE_CREATOR(name) static char* __MODULE_CREATOR __attribute__((unused)) = name;
#define MODULE_CONTACT(contact) static char* __MODULE_CONTACT __attribute__((unused)) = contact;
#define MODULE_LICENSE(license) static char* __MODULE_LICENSE __attribute__((unused)) = license;
#define mprintf( s, ... ) { printf("[%s] ", __MODULE_NAME); printf(s,__VA_ARGS__); }
#define mputs(s) { printf("[%s] %s", __MODULE_NAME, s); }
#define mdumpd() { printf("Module %s is written by %s <%s>, licensed under %s\n",__MODULE_NAME,__MODULE_CREATOR,__MODULE_CONTACT,__MODULE_LICENSE); }
#else
#define MODULE(name)
#define MODULE_CREATOR(name)
#define MODULE_CONTACT(contact)
#define MODULE_LICENSE(license)
#define mprintf( s, ... ) { printf(s,__VA_ARGS__); }
#define mputs(s) { printf("%s", s); }
#define mdumpd() 
#endif

#ifndef ONT_DO_MODULES
#define PANIC(why) { mprintf("***Kernel panic***\nfile: %s\nln: %i\nfunction: %s\nreason: %s ... hang on in there\n", __FILE__, __LINE__, __func__, why); mdumpd(); for(;;) {  }; }
#ifdef PANIC_ON_OOPS
#define OOPS(why) { mputs("Oops detected, panicing instead\n"); PANIC(why); }
#else
#define OOPS(why) { mprintf("***Kernel oops (undefined behaviour ahead)***\nfile: %s\nln: %i\nfunction: %s\nreason: %s\n", __FILE__, __LINE__, __func__, why); mdumpd(); }
#endif
#else
#define PANIC(why) { mprintf("***Kernel panic***\nfile: %s\nln: %i\nfunction: %s\nreason: %s ... hang on in there\n", __FILE__, __LINE__, __func__, why); for(;;) {  }; }
#ifdef PANIC_ON_OOPS
#define OOPS(why) { mputs("Oops detected, panicing instead\n"); PANIC(why); }
#else
#define OOPS(why) { mprintf("***Kernel oops (undefined behaviour ahead)***\nfile: %s\nln: %i\nfunction: %s\nreason: %s\n", __FILE__, __LINE__, __func__, why); }
#endif
#endif

#endif