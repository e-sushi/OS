namespace memory {

b32
initialize() {
    volatile static limine_memmap_request mreq = {
        .id = LIMINE_MEMMAP_REQUEST,
        .revision = 0,
    };

    kernel.memory = {0};

    if(!mreq.response) {
        kernel.serial.error("limine memmap request failed");
        return false;
    }

    kernel.serial.say("limine memmap request gave response: ", fmt::Hex((u64)mreq.response));

    auto response = mreq.response;

    kernel.serial.say("  there are ", response->entry_count, " entries");

    u64 usable_bytes = 0;

    Region* r = 0;

    fori(response->entry_count) {
        auto entry = (*response->entries)[i];
        auto type_names = [](u64 type) {
            switch(type) {
                case LIMINE_MEMMAP_USABLE:                 return "usable";
                case LIMINE_MEMMAP_RESERVED:               return "reserved";
                case LIMINE_MEMMAP_ACPI_RECLAIMABLE:       return "acpi_reclaimable";
                case LIMINE_MEMMAP_ACPI_NVS:               return "acpi_nvs";
                case LIMINE_MEMMAP_BAD_MEMORY:             return "bad_memory";
                case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE: return "bootloader_reclaimable";
                case LIMINE_MEMMAP_KERNEL_AND_MODULES:     return "kernel_and_modules";
                case LIMINE_MEMMAP_FRAMEBUFFER:            return "framebuffer";
            }
        };

        kernel.serial.say("    ", i, " base: ", fmt::Hex(entry.base), ", length: ", fmt::MemorySize(entry.length), ", type: ", type_names(entry.type));
        if(any(entry.type, LIMINE_MEMMAP_USABLE)) { 
            // TODO(sushi) reclaim ACPI and bootloader stuff eventually
            //             but NOT here of course
            usable_bytes += entry.length;

            Region* nu = (Region*)entry.base;
            *nu = {0};
            nu->prev = r;
            nu->free_blocks.next = nu->free_blocks.prev = &nu->free_blocks;
            Block* b = (Block*)(nu + 1);
            *b = {0};
            b->node.prev = b->node.next = &b->node;
            b->size = entry.base;
            nu->free_blocks.insert_next(&b->node);

            if(!r) kernel.memory.region = nu;
            r = nu;
        }
    }

    kernel.serial.say("  leaving us with ", fmt::MemorySize(usable_bytes), " of usable memory.");
    return true;
}

void*
allocate(u32 size) {
    Region* r = kernel.memory.region;
    while(1) {
        Block* b = block_from_node(r->free_blocks.next);
        while(1) {
            if(b->size >= size) {
                b->node.remove();
                Block* nu = (Block*)((u8*)b + size + sizeof(Block));
                *nu = {0};
                nu->size = b->size - size;
                nu->prev = b;
                nu->region = r;
                r->free_blocks.insert_last(&nu->node);
                b->size = size;
                b->allocated = true;
                return (u8*)b + allocated_block_size;
            }
            b = block_from_node(b->node.next);
            if((u8*)b > (u8*)r + r->size) break;
        }
        r = r->next;
        if(!r) {
            // TODO(sushi) someway to indicate error
            return 0;
        }
    }
}

template<typename T> T*
allocate() {
    return (T*)allocate(sizeof(T));
}

template<typename T> T*
allocate(u32 count) {
    return (T*)allocate(sizeof(T) * count);
}

void
free(void* ptr) {
    auto b = block_header(ptr);
    b->allocated = false;
    b->region->free_blocks.insert_next(&b->node);
    // check prev and next blocks and try to merge them if 
    // they're also free
    if(b->prev && !b->prev->allocated) { 
        b->prev->size += b->size + sizeof(Block);
        b->node.remove();
        b = b->prev;
    }

    Block* next = next_block(b);
    if(!next->allocated) {
        b->size += next->size + sizeof(Block);
        next->node.remove();
    }
}

void
move(void* dst, void* src, u32 size) {
    u8* s = (u8*)src;
    u8* d = (u8*)dst;

    if(s < d) {
        if(s + size < d) return copy(dst, src, size);
        fori_reverse(size) {
            d[i] = s[i];
        }
    } else if(d < s) copy(dst, src, size);
}

void
copy(void* dst, void* src, u32 size) {
    u32 divisor = 4 * sizeof(u32);
    u32 rounded = ((size - divisor/2)/divisor)*divisor;
    if(rounded > size) rounded = 0;
    fori(rounded/divisor) {
        asm volatile(
            "movups %0, %%xmm0\n"
            "movups %%xmm0, %1\n"
            :: "m" (*((u32*)src + 4*i)), "m" (*((u32*)dst + 4*i))
        );
    }
    for(int i = rounded; i < size; i++)
        ((u8*)dst)[i] = ((u8*)src)[i];
}

void
zero(void* dst, u32 size) {
    fori(size / 4) ((u32*)dst)[i] = 0;
    fori(size % 4) ((u8*)dst)[i] = 0;
}

} // namespace memory