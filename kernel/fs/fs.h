#ifndef FS_H
#define FS_H

#include "../io/block.h"

typedef struct {
    block_device* blk;
    char name[7]; // must be at most 6 chars long (and the trailing 0). examples: fatfs0 cdrom0
    void* (*open_file)(char* path);
    void* (*stat_file)(void* f);
    int (*close_file)(void* f);
    int (*write_file)(void* f, void* d, int l);
    int (*seek_file)(void* f, int l);
    int (*read_file)(void* f, void* d, int l);
} filesystem;

void* fopen(char* path);
int fclose(void* file);
int fwrite(void* f, void* d, int l);
int fseek(void* f, int l);
int fread(void* f, void* d, int l);

void initfs();

void add_fs(filesystem* fs);

#endif