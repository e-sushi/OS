#ifndef KERNEL_GRAPHICS_H
#define KERNEL_GRAPHICS_H

#include "common.h"
#include "multiboot.h"

// idk if this should be setup like this but it's this way so it 
// can be globally stored on the Kernel object
struct Graphics {
    struct {
        u8* address;
        u64 width;
        u64 height;
        u64 pitch;
        u16 bpp;
        u8  memory_model;
        u8  red_mask_size;
        u8  red_mask_shift;
        u8  green_mask_size;
        u8  green_mask_shift;
        u8  blue_mask_size;
        u8  blue_mask_shift;
    } framebuffer;
};


namespace graphics {

struct fcolor {
    f32 r, g, b;
};

b32
initialize();

void
draw_rect(u32 x, u32 y, u32 w, u32 h, fcolor col);

void
draw_character(u32 x, u32 y, u32* character_map, u32 width, u32 height, fcolor col);

void
clear_screen() {
    // multiboot::framebuffer_info* fbi = multiboot::mi.addresses.framebuffer_info;
    // memory::zero((void*)fbi->addr, fbi->height * fbi->pitch);
}




} // namespace graphics

#endif // KERNEL_GRAPHICS_H