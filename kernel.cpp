#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// the bootloader
#include "limine.h"

#include "common.h"
#include "multiboot.h"
#include "memory.h"
#include "graphics.h"
#include "print.h"
#include "descriptors.h"

// global printing object
Printer p;

#include "multiboot.cpp"
#include "memory.cpp"
#include "descriptors.cpp"

typedef struct color_desc {
    u8 r, g, b;
} color_desc;


static u32 next = 1;
u32 rand() {
    next = next * 1103515245 + 12345;
    return (u32)(next/65536) % 32768;
}

static volatile limine_framebuffer_request fbreq = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
};

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;
 
    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }
 
    return dest;
}
 
void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;
 
    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }
 
    return s;
}
 
void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;
 
    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }
 
    return dest;
}
 
int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;
 
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }
 
    return 0;
}


extern "C"
void kernel_main(u32 magic, u32 addr) {
    if(!fbreq.response || fbreq.response->framebuffer_count < 1) {
        // FUUUUUCK
        asm("cli");
        while(1) asm("hlt");
    }

    while(1) {}
    
    // multiboot::parse(addr);
    // memory::initialize();

    // p.buffer = memory::allocate<u8>(Kilobytes(12));
    // p.error("ahhh");

    // descriptors::setup_gdt();

    // *((u8*)0xB8000) = 'h';

    // memory::zero(p.buffer, Kilobytes(12));

    // while(1) {
    //     fori(rand() % 1024) {
    //         p.print((char)(rand() % ('~' - ' ') + ' '));
    //     }
    //     p.print("\n");
    // }

}