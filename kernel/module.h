#ifndef MODULE_H
#define MODULE_H

#include "io/stdio.h"

//static char* __MODULE_NAME __attribute((unused)) = "UNKNOWN MODULE";

// panic.c
int panic(char* reason);

#ifdef VERBOSE
#define DVERBOSE(x) { x; }
#else 
#define DVERBOSE(x) 
#endif

#ifndef MOD_FUNC_DETERMINE
#define MOD_FUNC_DETERMINE __func__
#endif

#ifndef ONT_DO_MODULES
/**
 * @brief Sets the module name.
 * 
 */
#define MODULE(name) static char* __MODULE_NAME __attribute__((unused)) = name;
/**
 * @brief Name of the creator of the module.
 * 
 */
#define MODULE_CREATOR(name) static char* __MODULE_CREATOR __attribute__((unused)) = name;
/**
 * @brief Contact address of the module creator.
 * 
 */
#define MODULE_CONTACT(contact) static char* __MODULE_CONTACT __attribute__((unused)) = contact;
/**
 * @brief Sets the license of the module. Should usually be AGPL.
 * 
 */
#define MODULE_LICENSE(license) static char* __MODULE_LICENSE __attribute__((unused)) = license;
/**
 * @brief Printf wrapper for modules.
 * 
 */
#ifndef O_NOT_PRINTFUNCNAMES
#define mprintf( s, ... ) { printf("[%s] %s: " s, __MODULE_NAME, MOD_FUNC_DETERMINE, __VA_ARGS__); }
/**
 * @brief Puts wrapper for modules.
 * 
 */
#define mputs(s) { printf("[%s] %s: %s", __MODULE_NAME, MOD_FUNC_DETERMINE, s); }
#else
#define mprintf( s, ... ) { printf("[%s] ONP: " s, __MODULE_NAME, __VA_ARGS__); }

#define mputs(s) { printf("[%s] ONP: %s", __MODULE_NAME, s); }
#endif
/**
 * @brief Dump module information.
 * 
 */
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
/**
 * @brief Kernel panic!
 * 
 */
#define PANIC(why) { mprintf("***Kernel panic***\nfile: %s\nln: %i\nfunction: %s\nreason: %s ... hang on in there\n\nbuild __DATE__:%s\n", __FILE__, __LINE__, __func__, why, __DATE__); mdumpd(); panic(why); }
#ifdef PANIC_ON_OOPS
#define OOPS(why) { mputs("Oops detected, panicing instead\n"); PANIC(why); }
#else
/**
 * @brief Oops alternative.
 * 
 */
#define OOPS(why) { mprintf("***Kernel oops (undefined behaviour ahead)***\nfile: %s\nln: %i\nfunction: %s\nreason: %s\n\nbuild __DATE__:%s\n", __FILE__, __LINE__, __func__, why, __DATE__); mdumpd(); }
#endif
#else
#define PANIC(why) { mprintf("***Kernel panic***\nfile: %s\nln: %i\nfunction: %s\nreason: %s ... hang on in there\n\nbuild __DATE__:%s\n", __FILE__, __LINE__, __func__, why, __DATE__); panic(why); }
#ifdef PANIC_ON_OOPS
#define OOPS(why) { mputs("Oops detected, panicing instead\n"); PANIC(why); }
#else
#define OOPS(why) { mprintf("***Kernel oops (undefined behaviour ahead)***\nfile: %s\nln: %i\nfunction: %s\nreason: %s\n\nbuild __DATE__:%s\n", __FILE__, __LINE__, __func__, why, __DATE__); }
#endif
#endif

#endif