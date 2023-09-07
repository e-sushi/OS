#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"
#include "multiboot.cpp"

#include "common.h"
#include "memory.h"
#include "print.h"

#include "memory.cpp"

typedef struct color_desc {
    u8 r, g, b;
} color_desc;


multiboot_tag* multiboot_find_tag(multiboot_tag* start, u32 type) {
    multiboot_tag* tag = (multiboot_tag*)((u8*)start + 8);

    while(1) {
        if(tag->type == 0 && tag->size == 8) {
            return 0;
        }

        if(tag->type == type) {
            return tag;
        }

        tag = (multiboot_tag*)((u8*)tag + ((tag->size + 7) & ~7));
    }
}

static u32 next = 1;
u32 rand() {
    next = next * 1103515245 + 12345;
    return (u32)(next/65536) % 32768;
}


extern "C"
void kernel_main(u32 magic, u32 addr) {
    u32* mymagic = (u32*)0xff;
    *mymagic = 0x5544;
    u32* place0 = (u32*)0x4f4f;
    u32* place1 = (u32*)0x5f5f;

    *place0 = magic;
    *place1 = addr;

    mem m;
    m.current = (u8*)0x9999;

    auto tag = (multiboot_tag*)addr;

    auto save = tag;
    auto size = tag->type;

    multiboot_tag* a = multiboot_find_tag(tag, 8);
 
    auto fbi = (multiboot_framebuffer_info*)(a + 1);

    auto coord = [&](u32 x, u32 y) {
        return (u8*)fbi->addr + y*fbi->pitch + x * fbi->bpp / 8;
    };

    
    while(1) {
        u32 top = rand() % fbi->height;
        u32 bottom = top + rand() % (fbi->height - top);
        u32 left = rand() % fbi->width;
        u32 right = left + rand() % (fbi->width - left);
        u32 col = 
            (rand() % 255 << 0) | 
            (rand() % 255 << 8) |
            (rand() % 255 << 16);
         for(int x = left; x < right; x++) {
            for(int y = top; y < bottom; y++) {
                *(u32*)coord(x, y) = col;
            }
        }
    }

    

   


    // u32* raaa = (u32*)fbi->addr;
    // u32 i = 0;
    // while(1) {
    //     u32 x = rand() % fbi->width;
    //     u32 y = rand() % fbi->height;

    //     *(u32*)coord(x,y) = 0xff000000;
    // }
}