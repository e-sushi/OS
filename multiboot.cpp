namespace multiboot {

multiboot_info mi;

void
parse(u32 addr) {
    mi.starting_addr = (u8*)addr;
    mi.total_size = *(u32*)addr;
    tag* curr = (tag*)(addr + 8);
    while(1) {
        if(!curr->type && curr->size == 8) return;

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
}

} // namespace multiboot