#ifndef JPROG_H
#define JPROG_H

#include <stdint.h>
#include <stdbool.h>

#define JPROG_EXECUTABLE 1<<1
#define JPROG_RELOCATABLE 1<<2

#define JPROG_UNKN 0
#define JPROG_CODE 1
#define JPROG_DATA 2

typedef struct {
    char name[16];
    uint8_t type;
    uint8_t flags;
    uint32_t length;
    uint32_t offset;
} jprog_shdr;

#define JPH_SIGNATURE "\xdaJpRoG"
#define JPH_SHDR_COUNT 32
typedef struct {
    char signature[7]; // 0xDA 'j' 'p' 'r' 'o' 'g' 0x00
    jprog_shdr shdrs[JPH_SHDR_COUNT];
    char machine_type;
    bool relocatable;
    char license[16];
    char author[32];
    char contact[32];
} jprog_hdr;

#endif