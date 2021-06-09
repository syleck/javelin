#ifndef JPROG_H
#define JPROG_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    char name[16];
    void* data;
} jprog_entry;

typedef struct {
    char name[16];
    uint8_t flags;
    jprog_entry entries[128];
} jprog_shdr;

typedef struct {
    char signature[4]; // 0xDA 'j' 'p' 'r'
    char machine_type;
    bool relocatable;
    jprog_shdr shdrs[3]; // should contain .text, .bss, .data
    uint32_t reservedA; // incsae if i do any extensions after release
    uint32_t reservedB;
    uint32_t reservedC;
} jprog_hdr;





#endif