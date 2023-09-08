#ifndef KERNEL_GRAPHICS_H
#define KERNEL_GRAPHICS_H

#include "common.h"
#include "multiboot.h"

namespace graphics {

struct fcolor {
    f32 r, g, b;
};

void
draw_character(u32 x, u32 y, u32* character_map, u32 width, u32 height, fcolor col) {
    // multiboot::framebuffer_info* fbi = multiboot::mi.addresses.framebuffer_info;
    // forx(yc, height) {
    //     forx(xc, width) {

    //         u32* px = (u32*)((u8*)fbi->addr + (y + yc) * fbi->pitch + (x + xc) * fbi->bpp / 8);
    //         *px = character_map[xc + yc * width];

    //         *((u8*)px + 2) *= col.r;
    //         *((u8*)px + 1) *= col.g;
    //         *((u8*)px + 0) *= col.b;
    //     }
    // }
}

void
clear_screen() {
    multiboot::framebuffer_info* fbi = multiboot::mi.addresses.framebuffer_info;
    memory::zero((void*)fbi->addr, fbi->height * fbi->pitch);
}

} // namespace graphics

#endif // KERNEL_GRAPHICS_H