namespace memory {

Controller controller;

void
initialize() {
    multiboot::memory_map* mm = multiboot::mi.addresses.memory_map;

    multiboot::memory_entry* entry = mm->entries;
    multiboot::tag* t = (multiboot::tag*)mm - 1;
    

    Region* r = (Region*)entry->base_addr;
    r->size = entry->length;
    if(!r) {
        // we need to push the region forward so that we aren't going to 
        // write over any important kernel info in the first ~ 1/2 mb of 
        // memory. So we just push it ahead by 5 mb. This is an arbitrary choice
        // as our boot asm starts at 1mb and I'm not currently sure how to 
        // start after it in a smart way :)
        r = (Region*)((u8*)r + Megabytes(5));
        r->size -= Megabytes(5);
    }

    r->free_blocks.next = r->free_blocks.prev = &r->free_blocks;
    Block* first = (Block*)(r + 1);
    first->node.next = first->node.prev = &first->node;
    first->size = r->size - sizeof(Block);
    first->region = r;
    first->allocated = false;
    r->free_blocks.insert_last(&first->node);

    controller.region = r;
    Region* last_region = r;

    while(1) {
        entry = (multiboot::memory_entry*)((u32)entry + mm->entry_size);
        if((u8*)entry >= (u8*)t + t->size) break;
        // ignore entries that are below our first region
        // since we don't want to write below 5 mb
        if(entry->base_addr < controller.region) continue;
        if(!any(entry->type, 1, 3, 4, 5)) continue; // skip any unusable entries

        r = (Region*)entry->base_addr;
        *r = Region();
         Block* first = (Block*)(r + 1);
        *first = Block();
        first->size = r->size - sizeof(Block);
        first->region = r;
        first->allocated = false;
        r->free_blocks.insert_last(&first->node);

        last_region->next = r;
        r->prev = last_region;

        last_region = r;

    }
}

void*
allocate(u32 size) {
    Region* r = controller.region;
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
copy(void* dst, void* src, u32 size) {
    for(u32 i = 0; i < size; i++) 
        ((u8*)dst)[i] = ((u8*)src)[i];
}

} // namespace memory