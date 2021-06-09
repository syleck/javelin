#include "device.h"
#include "../string.h"
#include "../module.h"
#include "../io/block.h"

MODULE("DEVICE");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

device_t devices[8192];
int device = 0;

io_struct null_device = {

};

device_t* alloc_device() {
    device_t* rts = &devices[device];
    device++;
    return rts;
}

int add_simple_text(char* name, io_struct io) {
    mprintf("New text driver: %s\n",name);
    devices[device].dev_type = DEVICE_TEXT;
    devices[device].io = io;
    devices[device].name = name;
    device++;
    return device-1;
}

int add_simple_block(char* name, block_device block) {
    mprintf("New block device: %s\n",name);
    devices[device].dev_type = DEVICE_DRIVE;
    devices[device].priv = &block;
    devices[device].name = name;
    device++;
    return device-1;
}

int add_alias(device_t* dev, char* alias) {
    mprintf("New driver alias: %s -> (orig)%s\n",alias,dev->name);
    devices[device].dev_type = DEVICE_ALIAS;
    devices[device].priv = dev;
    devices[device].io = null_device;
    devices[device].name = alias;
    return device-1;
}

device_t* get_device_byname(char* name) {
    for(int i = 0; i < 8192; i++) {
        if(strcmp(name,devices[i].name)==0) {
            return get_device(i);
        }
    }
    mprintf("WARN: Couldnt find any devices named %s\n",name);
}

device_t* get_device_bytype(int type) {
    for(int i = 0; i < 8192; i++) {
        if(devices[i].dev_type == type) {
            return &devices[i];
        }
    }
    mprintf("WARN: Couldnt find any devices for type %x\n",type);
}

device_t* get_device(int id) {
    device_t* cdev = &devices[id];
    while(cdev->dev_type == DEVICE_ALIAS) {
        cdev = (device_t*)cdev->priv;
    }
    return cdev;
}

void dbg_list_devices() {
    for(int i = 0; i < 8192; i++) {
        if(devices[i].dev_type != 0) {
            mprintf("Device %i:\n    Name: %s Priv: %x Type: %x Io: %x\n", i, devices[i].name, devices[i].priv, devices[i].dev_type, devices[i].io);
            if(devices[i].dev_type == DEVICE_ALIAS) {
                mprintf("Alias to device %s\n",get_device(i)->name);
            }
        }
    }
}