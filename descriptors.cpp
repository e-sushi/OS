namespace descriptors {

// see: https://wiki.osdev.org/GDT_Tutorial#Filling_the_Table
void
encode_gdt_entry(u8* target, GDTEntry src) {
    if(src.limit > 0xfffff) 
        p.error("GDT cannot encode limits larger than 0xfffff!");
    
    p.print("encoding gdt entry: \n",
            "   limit: ", Hex(src.limit), "\n",
            "    base: ", Hex(src.base), "\n",
            "  access: ", Hex(src.access), "\n",
            "   flags: ", Hex(src.flags), "\n");

    // encode limit
    target[0] =  src.limit        & 0xff;
    target[1] = (src.limit >> 8 ) & 0xff;
    target[6] = (src.limit >> 16) & 0x0f;

    // encode base
    target[2] =  src.base        & 0xff;
    target[3] = (src.base >> 8 ) & 0xff;
    target[4] = (src.base >> 16) & 0xff;
    target[7] = (src.base >> 24) & 0xff;

    // encode access byte
    target[5] = src.access;

    // encode the flags
    target[6] |= src.flags << 4;
}

void
setup_gdt() {
    // for now we just setup the basic 32bit GDT described in 
    // https://wiki.osdev.org/GDT_Tutorial
    // this isn't actually segmenting anything 
    // I'll come back later and setup something better if it seems necessary

    // u8* table = memory::allocate<u8>(8 * 6);

    // //null descriptor 
    // encode_gdt_entry(table + 0, {0});

    // // kernel mode code segment
    // encode_gdt_entry(table + 8, {
    //     .base = 0, 
    //     .limit = 0xfffff, 
    //     .access = 0x9a, 
    //     .flags = 0xc,
    // });

    // // kernel mode data segment 
    // encode_gdt_entry(table + 16, {
    //     .base = 0,
    //     .limit = 0xfffff,
    //     .access = 0x92,
    //     .flags = 0xc,
    // }); 

    // // user mode code segment
    // encode_gdt_entry(table + 24, {
    //     .base = 0,
    //     .limit = 0xfffff,
    //     .access = 0xfa, 
    //     .flags = 0xc,
    // });

    // // user mode data segment
    // encode_gdt_entry(table + 32, {
    //     .base = 0,
    //     .limit = 0xfffff,
    //     .access = 0xf2,
    //     .flags = 0xc,
    // });

    // // im ignoring the TSS for now but it MIGHT be necessary idk yet 
    // // looks like it will be when i want to do task switching (duh)

    // set_gdt(8 * 6, (u32)table);
}

} // namesapce descriptors