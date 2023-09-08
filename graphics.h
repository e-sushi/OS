#ifndef KERNEL_GRAPHICS_H
#define KERNEL_GRAPHICS_H

#include "common.h"
#include "multiboot.h"

namespace graphics {

void
draw_character(u32 x, u32 y, u32* character_map, u32 width, u32 height) {
    multiboot::framebuffer_info* fbi = multiboot::mi.addresses.framebuffer_info;
    forx(yc, height) {
        forx(xc, width) {
            u32* px = (u32*)((u8*)fbi->addr + (y + yc) * fbi->pitch + (x + xc) * fbi->bpp / 8);
            *px = character_map[xc + yc * width];
        }
    }
}

} // namespace graphics

#endif // KERNEL_GRAPHICS_H