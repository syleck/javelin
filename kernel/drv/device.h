#ifndef DEVICE_H
#define DEVICE_H
#include <stdint.h>
#include "../io/stdio.h"
#define DEVICE_BLOCK 0xB10C4000
#define DEVICE_DRIVE 0xD414E000
#define DEVICE_TEXT  0x1E410000
#define DEVICE_MISC  0x315C0000
#define DEVICE_ALIAS 0xA11A5000
#define DEVICE_PCI   0x3C100000

typedef struct {
    char* name;
    int dev_type;
    void* priv;
    io_struct io;
} device_t;

device_t* alloc_device();
int add_simple_text(char* name, io_struct io);
int add_alias(device_t* dev, char* alias);
device_t* get_device(int id);
device_t* get_device_byname(char* name);
device_t* get_device_bytype(int type);

#endif