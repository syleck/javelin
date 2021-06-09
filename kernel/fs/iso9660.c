#include "iso9660.h"
#include "../module.h"
#include <stdint.h>

MODULE("ISO9660")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

typedef uint8_t int8;
typedef int8_t sint8;
typedef uint16_t int16_LSB;
typedef uint16_t int16_MSB;
typedef struct { int16_LSB lsb; int16_MSB msb; } int16_LSB_MSB;
typedef int16_t sint16_LSB;
typedef int16_t sint16_MSB;
typedef struct { sint16_LSB lsb; sint16_MSB msb; } sint16_LSB_MSB;
typedef uint32_t int32_LSB;
typedef uint32_t int32_MSB;
typedef struct { int32_LSB lsb; int32_MSB msb; } int32_LSB_MSB;
typedef int32_t sint32_LSB;
typedef int32_t sint32_MSB;
typedef struct { sint32_LSB lsb; sint32_MSB msb; } sint32_LSB_MSB;

typedef struct {
    int8 type;
    char identifier[5];
    int8 version;
    char data[2041];
} primary_volume_descriptor;

primary_volume_descriptor pvd;

filesystem* init_iso9660(block_device* blk) {
    mprintf("FS started on block device %x with block size of %i\n",blk,blk->block_size);
    if(blk->type == BDT_ATAPIO)
        mputs("Warn: Using atapio block device\n");
    blk->read_block(1,&pvd,2);
    mprintf("Read pvd, type: %i, version: %i\n", pvd.type, pvd.version);
}