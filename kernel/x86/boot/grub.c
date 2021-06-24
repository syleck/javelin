#include "grub.h"
#include "../../sys/state.h"
#include "../../module.h"
#include <stdbool.h>
#include "mbtag.h"

MODULE("MB-GRUB2")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

struct multiboot_tag_elf_sections* sections;

void multiboot_init(uint32_t eax, uint32_t ebx) {
    mprintf("Boot EAX: %x, EBX %x\n",eax,ebx);
    if(eax != 0x36d76289) {
        DVERBOSE(mprintf("Not loaded through a valid multiboot2 bootloader, aborting (EAX = %x)\n",eax));
    }
    struct multiboot_tag *header = (void*)ebx;
    mprintf("Header size: %x, Reserved: %x\n", header->type, header->size);
    ebx += sizeof(struct multiboot_tag);
    //if (ebx % 8 != 0)
    //    ebx += 8 - ebx % 8;
    multiboot_u* current_tag = (void*)ebx;
    bool fault = false;
    for(current_tag = ebx; current_tag->tag.type != 0; current_tag = (struct multiboot_tag *) ((multiboot_uint8_t *) current_tag  + ((current_tag->tag.size + 7) & ~7))) {
        //mprintf("Tag type: %i, size: %x at %x\n",current_tag->tag.type,current_tag->tag.size,current_tag);
        switch(current_tag->tag.type) {
            case 1: // boot command line
                mprintf("Command line: %s\n",current_tag->string.string);
                break;
            case 2: // boot loader name
                mprintf("Bootloader: %s\n",current_tag->string.string);
                break;
            case 3: // module
                mprintf("Module %s (%x)\n",current_tag->module.cmdline,current_tag->module.mod_start);
                if(strcmp(current_tag->module.cmdline,"font")==0) {
                    set_info(SYSINFO_TEXT_FBFONT_START,current_tag->module.mod_start);
                    set_info(SYSINFO_TEXT_FBFONT_END,current_tag->module.mod_end);
                } else if(strcmp(current_tag->module.cmdline,"icon")==0) {
                    set_info(SYSINFO_ICON_START,current_tag->module.mod_start);
                    set_info(SYSINFO_ICON_END,current_tag->module.mod_end);
                }
                break;
            case 4: // memory information
                mprintf("memory information\nlowmem: %ikB, himem: %ikB\n",current_tag->meminfo.mem_lower,current_tag->meminfo.mem_upper);
                set_info(SYSINFO_MALLOC_START,0x01000000);
                set_info(SYSINFO_MALLOC_NOMEM,(current_tag->meminfo.mem_upper*1000)+0x01000000);
                break;
            case 5: // bios boot device

                break;
            case 6: // memory map
                mprintf("mmap sz: %i, esz: %i, v: %i\n",current_tag->mmap.size,current_tag->mmap.entry_size,current_tag->mmap.entry_version);

                break;
            case 8: // framebuffer info
                set_info(SYSINFO_VIDEO_ADDR,current_tag->framebuffer.common.framebuffer_addr);
                set_info(SYSINFO_VIDEO_BPP,current_tag->framebuffer.common.framebuffer_bpp);
                set_info(SYSINFO_VIDEO_RESY,current_tag->framebuffer.common.framebuffer_height);
                set_info(SYSINFO_VIDEO_RESX,current_tag->framebuffer.common.framebuffer_width);
                set_info(SYSINFO_VIDEO_PITCH,current_tag->framebuffer.common.framebuffer_pitch);
                break;
            case 9: // elf symbols
                sections = current_tag;
                break;
            case 10: // apm table
                break;
            case 21: // physical address
                set_info(SYSINFO_MEM_LOAD,current_tag->load_base_addr.load_base_addr);
                break;
            default:
                DVERBOSE(mprintf("I dont know what to do with this tag (%i)\n",current_tag->tag.type));
                break;
        }
        if(fault)
            break;
    }
    if(fault)
        DVERBOSE(mputs("Bad tag\n"));
    mputs("Done\n");
    trigger_update();
}

char* get_symbol(void* addr) {
    return "???";
}