namespace multiboot {

multiboot_info mi;

void
parse(u32 addr) {
    // start printing directly after the multiboot structure
    // this is probably not a good idea
    mi.starting_addr = (u8*)addr;
    mi.total_size = *(u32*)addr;
    tag* curr = (tag*)(addr + 8);

    // temporarily set the printer to the address after the multiboot header until we
    // can use the memory system to put it somewhere better afterwards
    p = Printer((u8*)addr + mi.total_size);

    while(1) {
        if(!curr->type && curr->size == 8) break;

        switch(curr->type) {
            case memory_map_type: {
                mi.addresses.memory_map = (memory_map*)(curr + 1);
            } break;
            case boot_loader_name_type: {
                mi.addresses.boot_loader_name = (boot_loader_name*)(curr + 1);
            } break;
            case framebuffer_info_type: {
                mi.addresses.framebuffer_info = (framebuffer_info*)(curr + 1);
            } break;
            // TODO(sushi) support more multiboot stuff as needed
        }
        curr = (tag*)((u8*)curr + ((curr->size + 7) & ~7));
    }

    p.print("finished parsing multiboot info from addr 0x", Hex(addr), "\n");
    if(mi.addresses.boot_loader_name) {
           p.print("found bootloader name tag at 0x", Hex((u32)mi.addresses.memory_map), "\n");
           p.print("bootloader is: \n");
           p.print("    ", (char*)mi.addresses.boot_loader_name->str, "\n");
    }
    if(mi.addresses.memory_map) {
        p.print("found memory map tag at 0x", Hex((u32)mi.addresses.memory_map), "\n");
    }
    if(mi.addresses.framebuffer_info) { // gee I sure hope we did or this was all for nothing
        p.print("found framebuffer tag at 0x", Hex((u32)mi.addresses.framebuffer_info), "\n");
    }
}

} // namespace multiboot