#include "fs.h"
#include "../stdlib.h"
#include "../mem.h"
#include "../string.h"
#include "../module.h"

MODULE("FS");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

filesystem *filesystems[8192];

// URI:
// filesy/path/dir/file.txt
struct uri decode_uri(char* url) {
    struct uri ur;
    char device[7];
    memcpy(device,url,6); 
    for(int i = 0; i < filesystems; i++) {
        if(strcmp(device,filesystems[i]->name) == 0) {
            ur.fs = filesystems[i];
        }
    }
    ur.path = malloc(strlen(url)-6);
    memcpy(ur.path,url+6,strlen(url)-6);
    mprintf("\"%s\" -> %s on %s (%s)",url,ur.path,ur.fs->name,device);
}

fs_path decode_path(char* path) {

}

void* fopen(char* path) {
    struct uri ur = decode_uri(path);
    fs_path pth = decode_path(ur.path);
    
}