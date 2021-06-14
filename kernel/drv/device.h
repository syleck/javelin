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

/**
 * @brief Allocates a new device.
 * 
 * @return device_t* Pointer to device.
 */
device_t* alloc_device();
/**
 * @brief Turns an io_struct into a device.
 * 
 * @param name Name of device.
 * @param io io_struct.
 * @return int Id of device.
 */
int add_simple_text(char* name, io_struct io);
/**
 * @brief Adds a device alias. (ex, x86random or psurandom becomes random)
 * 
 * @param dev Device pointer.
 * @param alias Alias.
 * @return int 
 */
int add_alias(device_t* dev, char* alias);
/**
 * @brief Get the device object
 * 
 * @param id 
 * @return device_t* 
 */
device_t* get_device(int id);
/**
 * @brief Get the device byname
 * 
 * @param name 
 * @return device_t* 
 */
device_t* get_device_byname(char* name);
/**
 * @brief Get the device bytype
 * 
 * @param type 
 * @return device_t* 
 */
device_t* get_device_bytype(int type);

#endif