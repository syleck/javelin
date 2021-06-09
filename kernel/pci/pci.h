#ifndef PCI_H
#define PCI_H

#include <stdint.h>

typedef union {
    uint32_t l;
    struct {
        uint8_t l;
        uint8_t l1;
        uint8_t h;
        uint8_t h1;
    } b;
    struct {
        uint16_t l;
        uint16_t h;
    } w;
} stupidint32; // im too lazy to use & 0xFF or nay of that

typedef struct {
    int function;
    int bus;
    int slot;
    // reg 0
    uint16_t vendor;
    uint16_t device;
    // reg 1
    uint16_t command;
    uint16_t status;
    // reg 2
    uint8_t revision;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class;
    // reg 3
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
} pci_header_base;

typedef struct {
    int function;
    int bus;
    int slot;
    // reg 0
    uint16_t vendor;
    uint16_t device;
    // reg 1
    uint16_t command;
    uint16_t status;
    // reg 2
    uint8_t revision;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class;
    // reg 3
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
    // reg 4
    uint32_t bar0;
    // reg 5
    uint32_t bar1;
    // reg 6
    uint32_t bar2;
    // reg 7
    uint32_t bar3;
    // reg 8
    uint32_t bar4;
    // reg 9
    uint32_t bar5;
    // reg A
    uint32_t cardbus_cis;
    // reg B
    uint16_t subsystem_vendor;
    uint16_t subsystem_id;
    // reg C
    uint32_t expansion_rom;
    // reg D
    uint8_t capabilities;
    uint16_t reserved0;
    uint8_t reserved1;
    // reg E
    uint32_t reserved2;
    // reg F
    uint8_t interrupt;
    uint8_t interrupt_pin;
    uint8_t min_grant;
    uint8_t max_latency;
} pci_header0;

typedef union upci_table_u {
    pci_header_base base;
    pci_header0 header0;
} pci_table_u;

void setpciok(int slot, int bus, int function);
void pci_install();

#endif