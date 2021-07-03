#ifndef FAT_H
#define FAT_H
#include <stddef.h>
#include <stdint.h>

#include "fs.h"

//FAT constant values
#define END_CLUSTER_32 0x0FFFFFF8 //Use OSDev.org's suggestion of 0x0FFFFFF8 even though MSYS docs > OSdev.org.
#define BAD_CLUSTER_32 0x0FFFFFF7
#define FREE_CLUSTER_32 0x00000000
#define END_CLUSTER_16 0xFFF8
#define BAD_CLUSTER_16 0xFFF7
#define FREE_CLUSTER_16 0x0000
#define END_CLUSTER_12 0xFF8
#define BAD_CLUSTER_12 0xFF7
#define FREE_CLUSTER_12 0x000

#define CLEAN_EXIT_BMASK_16 0x8000
#define HARD_ERR_BMASK_16 0x4000
#define CLEAN_EXIT_BMASK_32 0x08000000
#define HARD_ERR_BMASK_32 0x04000000

#define FILE_READ_ONLY 0x01
#define FILE_HIDDEN 0x02
#define FILE_SYSTEM 0x04
#define FILE_VOLUME_ID 0x08
#define FILE_DIRECTORY 0x10
#define FILE_ARCHIVE 0x20
#define FILE_LONG_NAME (FILE_READ_ONLY|FILE_HIDDEN|FILE_SYSTEM|FILE_VOLUME_ID)
#define FILE_LONG_NAME_MASK (FILE_READ_ONLY|FILE_HIDDEN|FILE_SYSTEM|FILE_VOLUME_ID|FILE_DIRECTORY|FILE_ARCHIVE)
#define FILE_LAST_LONG_ENTRY 0x40
#define ENTRY_FREE 0xE5
#define ENTRY_END 0x00
#define ENTRY_JAPAN 0x05
#define LAST_LONG_ENTRY 0x40

#define LOWERCASE_ISSUE	0x01 //E.g.: "test    txt"
#define BAD_CHARACTER	0x02 //E.g.: "tes&t   txt"
#define BAD_TERMINATION 0x04 //missing null character at the end
#define NOT_CONVERTED_YET 0x08 //still contains a dot: E.g."test.txt"
#define TOO_MANY_DOTS 0x10 //E.g.: "test..txt"; may or may not have already been converted

#define GET_CLUSTER_FROM_ENTRY(x) (x.low_bits | (x.high_bits << (fat_type / 2)))
#define GET_ENTRY_LOW_BITS(x) (x & ((fat_type /2) -1))
#define GET_ENTRY_HIGH_BITS(x) (x >> (fat_type / 2))
#define CONCAT_ENTRY_HL_BITS(high, low) ((high << (fat_type / 2)) | low)

typedef struct fat_extBS_32 {
    uint32_t table_size_32;
    uint16_t extended_flags;
    uint16_t fat_version;
    uint32_t root_cluster;
    uint16_t fat_info;
    uint16_t backup_bs_sector;
    char reserved_0[12];
    uint8_t drive_number;
    uint8_t boot_signature;
    uint32_t volume_id;
    char volume_label[11];
    char fat_type_label[8];
} __attribute__((packed)) fat_extBS_32_t;

typedef struct fat_extBS_16 {
    uint8_t bios_drive_num;
    uint8_t reserved;
    uint8_t boot_signature;
    uint8_t volume_id;
    char label[11];
    char type_label[8];
} __attribute__((packed)) fat_extBS_16_t;

typedef struct fat_BS {
    uint8_t bootjmp[3];
    char oem[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t table_count;
    uint16_t root_entry_count;
    uint16_t total_sectors_16;
    uint8_t media_type;
    uint16_t table_size_16;
    uint16_t sectors_per_track;
    uint16_t head_side_count;
    uint32_t hidden_sector_count;
    uint32_t total_sectors_32;
    char extended_section[54];
} __attribute__((packed)) fat_BS_t;

typedef struct directory_entry {
    char filename[8];
    char extension[3];
    char attributes;
    char reserved0;
    char creation_time_tenths;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_accessed;
    uint16_t high_bits;
    uint16_t last_modification_time;
    uint16_t last_modification_date;
    uint16_t low_bits;
    uint32_t file_size;
} __attribute__((packed)) directory_entry_t;

typedef struct fsInfo {
    uint32_t lead_signature;
    char reserved1[480];
    uint32_t structure_signature;
    uint32_t free_space;
    uint32_t last_written;
    char reserved2[12];
    uint32_t trail_signature;
} __attribute__((packed)) fsInfo_t;

typedef struct long_entry {
    uint8_t order;
    char first_five[10];
    char attributes;
    char type;
    char checksum;
    char next_six[12];
    uint16_t zero;
    char last_two[4];
} __attribute__((packed)) long_entry_t;



void fat_init(block_device* blk);

#endif