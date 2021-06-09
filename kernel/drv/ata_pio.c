#include "ata_pio.h"
#include "device.h"
#include "../x86/asm.h"
#include "../module.h"
#include "../io/block.h"

MODULE("ATAPIO");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

#define STATUS_BSY 0x80
#define STATUS_RDY 0x40
#define STATUS_DRQ 0x08
#define STATUS_DF 0x20
#define STATUS_ERR 0x01

static void ATA_wait_BSY()   //Wait for bsy to be 0
{
	while(inb(0x1F7)&STATUS_BSY);
}
static void ATA_wait_DRQ()  //Wait fot drq to be 1
{
	while(!(inb(0x1F7)&STATUS_RDY));
}

void* read_sectors_ATA_PIO(uint32_t target_address, uint32_t LBA, uint8_t sector_count)
{
	mprintf("Reading %i sectors from lba %i to %x\n", sector_count, LBA, target_address);

	ATA_wait_BSY();
	outb(0x1F6, 0xE0 | ((LBA >>24) & 0xF));
	outb(0x1F2, sector_count);
	outb(0x1F3, (uint8_t) LBA);
	outb(0x1F4, (uint8_t)(LBA >> 8));
	outb(0x1F5, (uint8_t)(LBA >> 16)); 
	outb(0x1F7, 0x20); //Send the read command

	uint16_t *target = (uint16_t*) target_address;

	for (int j =0;j<sector_count;j++)
	{
		ATA_wait_BSY();
		ATA_wait_DRQ();
		for(int i=0;i<ATA_BLOCK_SIZE;i++)
			target[i] = inb(0x1F0);
		target+=ATA_BLOCK_SIZE;
	}
	return (void*)target_address;
}

void* write_sectors_ATA_PIO(uint32_t LBA, uint8_t sector_count, uint32_t* bytes)
{
	mprintf("Writing %i sectors from %x to lba %i\n", sector_count, bytes, LBA);
	ATA_wait_BSY();
	outb(0x1F6, 0xE0 | ((LBA >>24) & 0xF));
	outb(0x1F2, sector_count);
	outb(0x1F3, (uint8_t) LBA);
	outb(0x1F4, (uint8_t)(LBA >> 8));
	outb(0x1F5, (uint8_t)(LBA >> 16)); 
	outb(0x1F7, 0x30); //Send the write command

	for (int j =0;j<sector_count;j++)
	{
		ATA_wait_BSY();
		ATA_wait_DRQ();
		for(int i=0;i<ATA_BLOCK_SIZE;i++)
		{
			outb(0x1F0, bytes[i]);
		}
	}
	return (void*)bytes;
}

void blkd_wrapper_read(int blk, void* addr, int c) {
	read_sectors_ATA_PIO(addr,blk,c);
}

void blkd_wrapper_write(int blk, void* addr, int c) {
	write_sectors_ATA_PIO(blk,c,addr);
}

block_device block = {
	.block_size = ATA_BLOCK_SIZE,
	.type = BDT_ATAPIO,
	.read_block = blkd_wrapper_read,
	.write_block = blkd_wrapper_write
};

void init_ata_pio() {
	add_simple_block("ata_pio", block);
	mputs("Finished initializing\n");
}