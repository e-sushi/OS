/*

    Primary structure of the kernel

*/

#ifndef KERNEL_KERNEL_H
#define KERNEL_KERNEL_H

#include "common.h"
#include "io.h"
#include "memory.h"
#include "graphics.h"
#include "print.h"
#include "descriptors.h"

struct Kernel {
    io::SerialPort serial;
    Printer printer;
    Memory memory;
    Graphics graphics;
};

extern Kernel kernal;

#endif // KERNEL_KERNEL_H