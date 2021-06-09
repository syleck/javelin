#ifndef BLOCK_H
#define BLOCK_H

#define BDT_GENERIC 0
#define BDT_ATA     1
#define BDT_ATAPI   2
#define BDT_ATAPIO  3

typedef struct {
    int block_size;
    int type;
    void (*read_block)(int blk, void* addr, int c);
    void (*write_block)(int blk, void* addr, int c);
} block_device;

#endif