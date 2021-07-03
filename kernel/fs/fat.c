#include "fat.h"
#include "../module.h"
#include "../mem.h"

MODULE("FS_FAT");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

enum { UNINITIALIZED, FAT12, FAT16, FAT32 } fat_type = UNINITIALIZED;
int first_fat_sector;
int first_data_sector;
int total_clusters;
fat_BS_t* bootsect;

int fat_read(filesystem* fs, int clusternum) {
    if(fat_type == UNINITIALIZED) {
        mputs("fat not yet initialized");
        return -1;
    }
    if(clusternum < 2 || clusternum > total_clusters) {
        mprintf("invalid cluster number (%i, max %i)\n",clusternum,total_clusters);
        return -1;
    }
    if(fat_type == FAT32) {
        int cluster_size = bootsect->bytes_per_sector * bootsect->sectors_per_cluster;
        char fat_table[32*1024] = { '\0' };
        int fat_offset = clusternum * 4;
        int fat_sector = first_fat_sector + (fat_offset / cluster_size);
        int ent_offset = fat_offset % cluster_size;

        fs->blk->read_block(fat_sector, kmalloc(fs->blk->block_size,"FatSector"), 1);

        int table_value = *(int*)&fat_table[ent_offset] & 0x0FFFFFFF;

        return table_value;
    } else if(fat_type == FAT16) {

    } else {
        mprintf("invalid FAT type %i!?!\n", fat_type);
    }
}

int cluster_read(int clusternum, int clusteroff) {
    if(fat_type == UNINITIALIZED) {
        mputs("fat not yet initialized");
        return -1;
    }

    int start_sect = (clusternum - 2) * (uint16_t)bootsect->sectors_per_cluster + first_data_sector;

    //fs->blk->read_block(start_sect, )
}

void fat_fopen(char* path) {
    
}

void fat_fread(void* f, void* d, int l) {

}

void fat_fclose(void* f) {

}

void fat_init(block_device* blk) {
    bootsect = kmalloc(blk->block_size,"FatSector");
    blk->read_block(0,bootsect,1);
    filesystem fatfs = {
        .name = "fatfs",
        .open_file = fat_fopen,
        .close_file = fat_fclose,
        .read_file = fat_fread,
    };
    add_fs(&fatfs);

    total_clusters = bootsect->total_sectors_16 / bootsect->sectors_per_cluster;
    if(total_clusters == 0) {
        total_clusters = bootsect->total_sectors_32 / bootsect->sectors_per_cluster;
    }

    if(total_clusters < 4085) {
        fat_type = FAT12;
        first_data_sector = bootsect->reserved_sector_count + bootsect->table_count * bootsect->table_size_16 + (bootsect->root_entry_count * 32 + bootsect->bytes_per_sector - 1) / bootsect->bytes_per_sector;
    } else {
        if(total_clusters < 65525) {
            fat_type = FAT16;
            first_data_sector = bootsect->reserved_sector_count + bootsect->table_count * bootsect->table_size_16 + (bootsect->root_entry_count * 32 + bootsect->bytes_per_sector - 1);
        } else {
            fat_type = FAT32;
            first_data_sector = bootsect->reserved_sector_count + bootsect->table_count * ((fat_extBS_32_t*)(bootsect->extended_section))->table_size_32;
        }
    }
    first_fat_sector = bootsect->reserved_sector_count;
}
