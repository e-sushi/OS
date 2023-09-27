
#ifndef KERNEL_DESCRIPTORS_H
#define KERNEL_DESCRIPTORS_H

#include "common.h"

// defined in boot.s
extern "C" void set_gdt(u16 limit, u32 gdt);

namespace descriptors {

struct GDTEntry {
    u32 base;
    u32 limit;
    u8  access;
    u8  flags;
};

//  bit Interrupt Descriptor entry
struct IDTEntry {
    u16 offset_low_low; // low part of the offset representing the address of the entry point of the interrupt service routine 
    u16 selector;   // code segment selector in GDT 
    u8  ist : 3;    // offset into the interupt stack table  
    u8 : 5;
    /* The type of gate this ID represents
        0b1110: 64-bit interrupt gate
        0b1111: 64-bit trap gate
    */
    u8  gate_type : 4;
    u8  DPL : 2; // determines the CPU privilege levels allowed to access this interrupt via the INT instr.
    u8  P : 1; // present bit, must always be 1
    u16 offset_low_high;
    u32 offset_high;
    u32 zero; // reserved

};

void
encode_gdt_entry(u8* target, GDTEntry src);

void
setup_gdt();

void
setup_idt();
    
} // namespace descriptors

#endif // KERNEL_DESCRIPTORS_H