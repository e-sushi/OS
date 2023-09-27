/*

    Currently this consists of various helpers for doing I/O with things like serial ports

*/

#ifndef KERNEL_IO_H
#define KERNEL_IO_H

#include "common.h"

namespace io {

static const u16 COM1 = 0x3f8;

static inline void 
outb(u16 port, u8 val) {
    asm volatile( // idk how this shit works 
        "outb %0, %1" : : "a"(val), "Nd"(port): "memory"
    );
}

static inline u8
inb(u16 port) {
    u8 ret;
    asm volatile (
        "inb %1, %0" : "=a"(ret) : "Nd"(port) : "memory"
    );
    return ret;
}

// initializes serial ports
// for use with outx and inx
// see: https://wiki.osdev.org/Serial_Ports#Programming_the_Serial_Communications_Port
s32 
init_serial() {
    outb(COM1 + 1, 0x00); // disable all interrupts
    outb(COM1 + 3, 0x80); // enable DLAB
    outb(COM1 + 0, 0x03); // set baud rate divisor to 3 (lo) 38400 baud
    outb(COM1 + 1, 0x00); //                            (hi)
    outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xc7); // enable FIFO, clear then with 14-byte threshold
    outb(COM1 + 4, 0x0b); // IRQs enabled, RTS/DSR set
    outb(COM1 + 4, 0x1e); // set in loopback mode for testing 
    outb(COM1 + 0, 0xae); // test serial chip by sending byte 0xae

    if(inb(COM1) != 0xae) {
        return 1;
    }

    // if it works, set back to normal operations
    outb(COM1 + 4, 0x0f);
    return 0;
}

// simple object to wrap serial port communication
// the provided port must have already been initialized before using this to send/receive data
struct SerialPort {
    u16 port;

    SerialPort() {}
    SerialPort(u16 port) : port(port) {}

    FORCE_INLINE
    void out(u8 x) { outb(port, x); }

    FORCE_INLINE
    u8 in() { 
        // poor man's wait for input
        while(!(inb(port + 5) & 1)) {} 
        return inb(port); 
    }

    template<typename... T> void
    print(T... args) {
        u8 buffer[Kilobytes(8)]; // YEAH WE NEED SOME ROOM BABE
        u32 count = to_str(buffer, args...);
        fori(count) {
            if(buffer[i] == '\n') {
                out(0x0a);
                out(0x0d);
            } else {
                out(buffer[i]);
            }
        }
    }

    template<typename... T> void
    error(T... args) {
        print("ERROR: ", args...);
    }

    template<typename... T> void
    say(T... args) {
        print(args..., "\n");
    }
};

} // namespace io

#endif // KERNEL_IO_H