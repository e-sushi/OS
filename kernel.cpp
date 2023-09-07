#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "common.h"
#include "multiboot.h"
#include "memory.h"

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

    void* A = memory::allocate(128);
    void* B = memory::allocate(512);
    void* C = memory::allocate(1024);
    void* D = memory::allocate(512);
    void* E = memory::allocate(128);

    memory::free(C);
    memory::free(D);
    memory::free(B);

}