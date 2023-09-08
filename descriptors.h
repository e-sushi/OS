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

// 32 bit Interrupt Descriptor entry
struct IDTEntry {
    u16 offset_low; // low part of the offset representing the address of the entry point of the interrupt service routine 
    u16 selector;   // code segment selector in GDT 
    u8  zero;       // unused and set to zero
    /* The type of gate this ID represents
        0x5: Task gate (offset value is unused)
        0x6: 16 bit interrupt gate
        0x7: 16 bit trap gate
        0xe: 32 bit interrupt gate
        0xf: 32 bit trap gate
    */
    u8  gate_type : 4;
    u8  DPL : 2; // determines the CPU privilege levels allowed to access this interrupt via the INT instr.
    u8  P : 1; // present bit, must always be 1
};

void
encode_gdt_entry(u8* target, GDTEntry src);

void
setup_gdt();
    
} // namespace descriptors