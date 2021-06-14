#include "grub.h"
#include "../../module.h"
#include <stdbool.h>
#include "mbtag.h"

MODULE("MB-GRUB2")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

void multiboot_init(uint32_t eax, uint32_t ebx) {
    mprintf("Boot EAX: %x, EBX %x\n",eax,ebx);
    if(eax != 0x36d76289) {
        mprintf("Not loaded through a valid multiboot2 bootloader, aborting (EAX = %x)\n",eax);
    }
    struct multiboot_tag *header = (void*)ebx;
    mprintf("Header size: %x, Reserved: %x\n", header->type, header->size);
    ebx += sizeof(*header);
    if (ebx % 8 != 0)
        ebx += 8 - ebx % 8;
    struct multiboot_tag* current_tag = (void*)ebx;
    bool fault = false;
    for(int i = 0; current_tag->type != 0; i++) {
        mprintf("Tag: %i, type: %i, size: %x\n",i,current_tag->type,current_tag->size);
        switch(current_tag->type) {
            case 0x0:
                break;
            case 24:
                return;
            default:
                mputs("I dont know what to do with this tag\n");
                break;
        }
        if(fault)
            break;
        current_tag += current_tag->size;
        if ((uint32_t)current_tag % 8 != 0) {
            mprintf("Addr not aligned %x\n",current_tag);
            current_tag += 8 - (uint32_t)current_tag % 8;
        }
    }
    if(fault)
        mputs("Bad tag\n");
}