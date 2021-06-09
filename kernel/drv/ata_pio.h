#ifndef ATA_H
#define ATA_H
#include <stdint.h>

void* read_sectors_ATA_PIO(uint32_t target_address, uint32_t LBA, uint8_t sector_count);
void* write_sectors_ATA_PIO(uint32_t LBA, uint8_t sector_count, uint32_t* bytes);

void init_ata_pio();

#define ATA_BLOCK_SIZE 256

#endif