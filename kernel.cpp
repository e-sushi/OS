#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "common.h"
#include "multiboot.h"
#include "memory.h"
#include "graphics.h"
#include "print.h"

#include "multiboot.cpp"
#include "memory.cpp"

typedef struct color_desc {
    u8 r, g, b;
} color_desc;


static u32 next = 1;
u32 rand() {
    next = next * 1103515245 + 12345;
    return (u32)(next/65536) % 32768;
}


extern "C"
void kernel_main(u32 magic, u32 addr) {
    multiboot::parse(addr);
    memory::initialize();

    u8* print_buffer = memory::allocate<u8>(Kilobytes(12));

    volatile u8 i;
    while(1) {
        print(print_buffer, (char)(rand() % ('~' - ' ') + ' '));
        i += 1;
    }
    print(print_buffer, i);
}