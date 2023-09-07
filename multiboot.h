#define MULTIBOOT2_HEADER_MAGIC 0xE85250D6

#include "string.h"

namespace multiboot {

struct tag {
    u32 type;
    u32 size;
};

struct memory_info {
    // amount of memory starting from address 0 in kilobytes 
    u32 lower;
    // amount of memory starting from address 1 Mb in kilobytes
    u32 upper;
};

// TODO(sushi) maybe handle this
struct bios_boot_device {
    u32 biosdev;
    u32 partiton;
    u32 sub_partition;
};

// I don't know what this actually does 
struct boot_command_line {
    u8* str;
};

struct modules {
    u32 mod_start;
    u32 mod_end;
    u8* str;
};

// TODO(sushi)
struct elf_symbols {
    u16 num;
    u16 entsize;
    u16 shndx;
    u16 reserved;
};

struct memory_entry {
    // starting physical address
    u64 base_addr;
    // size of memory region in bytes
    u64 length;
    // variety of address range this entry represents
    // 1: available RAM
    // 2: 3: usable memory holding ACPI info (?)
    // 4: reserved memory that needs to be preserved on hibernation
    // 5: memory occupied by defective RAM modules and all other values currently indicated in a reserved area
    u32 type;
    u32 reserved;
};

const u32 memory_map_type = 6;
struct memory_map {
    // the size of one entry
    u32 entry_size;
    // currently always 0    
    u32 entry_version;
    memory_entry entries[];
};

const u32 boot_loader_name_type = 2;
struct boot_loader_name {
    u8* str;
};

struct apm_table; // TODO(sushi)

struct vbe_info; // TODO(sushi)

const u32 framebuffer_info_type = 8;
struct framebuffer_info {
    u64 addr;
    u32 pitch;
    u32 width;
    u32 height;
    u8  bpp;
    u8  type;
    u8  reserved;
    union {
        struct {
            u32 n_colors;
            u8* colors; // TODO(sushi) this needs to point to an actual color struct when it is implemented
        } indexed_color;
        struct {
            u8 red_field_position;
            u8 red_mask_size;
            u8 green_field_position;
            u8 green_mask_size;
            u8 blue_field_position;
            u8 blue_mask_size;
        } direct_rgb;
    };
};


struct multiboot_info {
    u8* starting_addr;
    u32 total_size;
    struct {
        memory_info*       memory_info;
        bios_boot_device*  bios_boot_device;
        boot_command_line* boot_command_line;
        modules*           modules;
        elf_symbols*       elf_symbols; 
        memory_map*        memory_map;
        boot_loader_name*  boot_loader_name;
        framebuffer_info*  framebuffer_info;
    } addresses;
};

// parses the multiboot boot information
void 
parse(u32 addr);

extern multiboot_info mi;

} // namespace multiboot
