#include "fs.h"
#include "../stdlib.h"
#include "../mem.h"
#include "../string.h"
#include "../module.h"

MODULE("FS");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

#define MAX_FILESYSTEMS 8192
filesystem *filesystems[MAX_FILESYSTEMS];
filesystem* nullfs;
int filesystemc = 0;

void* fopen(char* path) {
    

}

void add_fs(filesystem* fs) {
    mprintf("new filesystem: %s\n",fs->name);
    filesystems[filesystemc++] = fs;
}

void initfs() {
    for(int i = 0; i < MAX_FILESYSTEMS; i++) {
        filesystems[i] = 0;
    }
    nullfs = malloc(sizeof(filesystem));
    memcpy(nullfs->name,"nullfs",7);
    add_fs(nullfs);
}