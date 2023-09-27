#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// the bootloader
#include "limine/limine.h"
#include "limine.h"

#include "common.h"
#include "cpu.h"
#include "io.h"
#include "memory.h"
#include "graphics.h"
#include "print.h"
#include "descriptors.h"
#include "kernel.h"

Kernel kernel;

#include "cpu.cpp"
#include "memory.cpp"
#include "graphics.cpp"
#include "print.cpp"
#include "descriptors.cpp"


typedef struct color_desc {
    u8 r, g, b;
} color_desc;


static u32 next = 1;
u32 rand() {
    next = next * 1103515245 + 12345;
    return (u32)(next/65536) % 32768;
}

void srand(u32 x) {
    next = x;
}

extern "C" void test(u64* a, u64* b);


extern "C"
void kernel_main(u32 magic, u32 addr) {
    if(io::init_serial()) {
        return;
    }
    kernel.serial.port = io::COM1;

    enable_sse();

    // clear the screen and return cursor to top left 
    kernel.serial.print("\033[2J");
    kernel.serial.print("\033[f");

    kernel.serial.say("parsing memory maps and setting up memory system...");
    if(!memory::initialize()) return;
    kernel.serial.say("memory acquisition complete!\n");

    kernel.serial.say("initializing graphics...");
    if(!graphics::initialize()) return;
    kernel.serial.say("graphics initialized!");

    kernel.printer.buffer = memory::allocate<u8>(Kilobytes(12));

    

    while(1) {}
}