#ifndef MBTAG_H
#define MBTAG_H

///
/// SHOULD ONLY REALLY BE USED BY GRUB.C
///

#include <stdint.h>

typedef uint32_t multiboot_uint64_t;
typedef uint32_t multiboot_uint32_t;
typedef uint16_t multiboot_uint16_t;
typedef uint8_t  multiboot_uint8_t;

struct multiboot_color
{
	multiboot_uint8_t red;
	multiboot_uint8_t green;
	multiboot_uint8_t blue;
};

struct multiboot_mmap_entry
{
	multiboot_uint64_t addr;
	multiboot_uint64_t len;
	#define MULTIBOOT_MEMORY_AVAILABLE              1
	#define MULTIBOOT_MEMORY_RESERVED               2
	#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
	#define MULTIBOOT_MEMORY_NVS                    4
	#define MULTIBOOT_MEMORY_BADRAM                 5
	multiboot_uint32_t type;
	multiboot_uint32_t zero;
};

struct multiboot_tag
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
};

struct multiboot_tag_string
{
	multiboot_uint32_t type;
	multiboot_uint32_t size;
	char string[0];
};

struct multiboot_tag_module
{
	multiboot_uint32_t type;
	multiboot_uint32_t size;
	multiboot_uint32_t mod_start;
	multiboot_uint32_t mod_end;
	char cmdline[0];
};

struct multiboot_tag_basic_meminfo
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t mem_lower;
  multiboot_uint32_t mem_upper;
};

struct multiboot_tag_bootdev
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t biosdev;
  multiboot_uint32_t slice;
  multiboot_uint32_t part;
};

struct multiboot_tag_mmap
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t entry_size;
  multiboot_uint32_t entry_version;
  struct multiboot_mmap_entry entries[0];  
};

struct multiboot_vbe_info_block
{
  multiboot_uint8_t external_specification[512];
};

struct multiboot_vbe_mode_info_block
{
  multiboot_uint8_t external_specification[256];
};

struct multiboot_tag_vbe
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;

  multiboot_uint16_t vbe_mode;
  multiboot_uint16_t vbe_interface_seg;
  multiboot_uint16_t vbe_interface_off;
  multiboot_uint16_t vbe_interface_len;

  struct multiboot_vbe_info_block vbe_control_info;
  struct multiboot_vbe_mode_info_block vbe_mode_info;
};

struct multiboot_tag_framebuffer_common
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;

  multiboot_uint64_t framebuffer_addr;
  multiboot_uint32_t framebuffer_pitch;
  multiboot_uint32_t framebuffer_width;
  multiboot_uint32_t framebuffer_height;
  multiboot_uint8_t framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB     1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT     2
  multiboot_uint8_t framebuffer_type;
  multiboot_uint16_t reserved;
};

struct multiboot_tag_framebuffer
{
  struct multiboot_tag_framebuffer_common common;

  union
  {
	struct
	{
	  multiboot_uint16_t framebuffer_palette_num_colors;
	  struct multiboot_color framebuffer_palette[0];
	};
	struct
	{
	  multiboot_uint8_t framebuffer_red_field_position;
	  multiboot_uint8_t framebuffer_red_mask_size;
	  multiboot_uint8_t framebuffer_green_field_position;
	  multiboot_uint8_t framebuffer_green_mask_size;
	  multiboot_uint8_t framebuffer_blue_field_position;
	  multiboot_uint8_t framebuffer_blue_mask_size;
	};
  };
};

struct multiboot_tag_elf_sections
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t num;
  multiboot_uint32_t entsize;
  multiboot_uint32_t shndx;
  char sections[0];
};

struct multiboot_tag_apm
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint16_t version;
  multiboot_uint16_t cseg;
  multiboot_uint32_t offset;
  multiboot_uint16_t cseg_16;
  multiboot_uint16_t dseg;
  multiboot_uint16_t flags;
  multiboot_uint16_t cseg_len;
  multiboot_uint16_t cseg_16_len;
  multiboot_uint16_t dseg_len;
};

struct multiboot_tag_efi32
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t pointer;
};

struct multiboot_tag_efi64
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint64_t pointer;
};

struct multiboot_tag_smbios
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint8_t major;
  multiboot_uint8_t minor;
  multiboot_uint8_t reserved[6];
  multiboot_uint8_t tables[0];
};

struct multiboot_tag_old_acpi
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint8_t rsdp[0];
};

struct multiboot_tag_new_acpi
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint8_t rsdp[0];
};

struct multiboot_tag_network
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint8_t dhcpack[0];
};

struct multiboot_tag_efi_mmap
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t descr_size;
  multiboot_uint32_t descr_vers;
  multiboot_uint8_t efi_mmap[0];
}; 

struct multiboot_tag_efi32_ih
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t pointer;
};

struct multiboot_tag_efi64_ih
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint64_t pointer;
};

struct multiboot_tag_load_base_addr
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t load_base_addr;
};

typedef union multiboot_union {
  struct multiboot_tag tag;
  struct multiboot_tag_string string;
  struct multiboot_tag_module module;
  struct multiboot_tag_basic_meminfo meminfo;
  struct multiboot_tag_bootdev bootdev;
  struct multiboot_tag_mmap mmap;
  struct multiboot_tag_vbe vbe;
  struct multiboot_tag_framebuffer framebuffer;
  struct multiboot_tag_elf_sections elf_sections;
  struct multiboot_tag_apm apm;
  struct multiboot_tag_efi32 efi32;
  struct multiboot_tag_efi64 efi64;
  struct multiboot_tag_smbios smbios;
  struct multiboot_tag_old_acpi old_acpi;
  struct multiboot_tag_new_acpi new_acpi;
  struct multiboot_tag_network network;
  struct multiboot_tag_efi_mmap efi_mmap;
  struct multiboot_tag_efi32_ih efi32_ih;
  struct multiboot_tag_efi64_ih efi64_ih;
  struct multiboot_tag_load_base_addr load_base_addr;
} multiboot_u;

#endif