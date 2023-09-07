namespace memory {

Controller controller;

void
initialize() {
    multiboot::memory_map* mm = multiboot::mi.addresses.memory_map;

    multiboot::memory_entry* entry = mm->entries;
    multiboot::tag* t = (multiboot::tag*)mm - 1;
    

    Region* r = (Region*)entry->base_addr;
    *r = {0};
    r->size = entry->length;
    r->first_free_block = (Block*)(r + 1);
    *r->first_free_block = {0};
    r->first_free_block->size = r->size - sizeof(Block);
    r->first_free_block->region = r;

    controller.region = r;
    Region* last_region = r;

    while(1) {
        entry = (multiboot::memory_entry*)((u32)entry + mm->entry_size);
        if((u8*)entry >= (u8*)t + t->size) break;

        r = (Region*)entry->base_addr;
        r->size = entry->length;
        r->first_free_block = (Block*)(r + 1);
        *r->first_free_block = {0};
        r->first_free_block->size = r->size - sizeof(Block);
        r->first_free_block->region = r;

        last_region->next = r;
        r->prev = last_region;

        last_region = r;

    }
}


void*
allocate(u32 size) {
    Region* r = controller.region;
    while(1) {
        Block* b = r->first_free_block;
        while(1) {
            if(b->size >= size) {
                Block* nu = (Block*)((u8*)b + size + sizeof(Block));
                *nu = {0};
                nu->size = b->size - size;
                nu->prev = b;
                nu->region = r;
                if(b == r->first_free_block) r->first_free_block = nu;
                b->next_free = nu->next_free;
                b->size = size;
                b->allocated = true;
                return (u8*)b + sizeof(Block) - sizeof(Block*);
            }
            b = (Block*)((u8*)b + b->size);
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
    auto b = (Block*)ptr - 1;
    if(b->prev) {
        if(!b->prev->allocated) {
            b->prev->size += b->size + sizeof(Block);
            b = b->prev;
        }
        if(b->region->first_free_block > b) {
            b->region->first_free_block = b;

        } else {

        }
    } else {
        ((Region*)b - 1)->first_free_block = b;

    }
    b->allocated = false;
}

FORCE_INLINE Block*
block_from_addr(void* addr) {
    return (Block*)addr - 1;
}

} // namespace memory

/*

R --------------------------------,
                                   v
|A    |B     |C     |D     |E     |Free                        |

R -----,
       v
|A    |Free  |C     |D     |E     |Free                        |
       |                           ^
       `--------------------------,

R -----,
       v
|A    |Free  |C     |Free  |E     |Free                        |
       |                           ^
       `--------------------------,
        R.f < D 

R -----,             , ------------,
       v             |             v
|A    |Free  |C     |Free  |E     |Free                        |
       |             ^
       `------------ ,
       
R -----,             , -------------------,
       v             |                    v
|A    |Free  |C     |Free  |E     |F     |Free                        |
       |             ^
       `------------ ,

R -----,             , --------------------------,
       v             |                           v
|A    |Free  |C     |Free  |E     |F     |G     |Free                        |
       | -----,       ^     |      |      |
       `------------ ,      ,      ,      ,
                      ^-------------------

(R
    Free)
(R
    Free
    A)
...
(R
    Free
    A
    B
    C
    D
    E)

(R
    (Free
        E
        D
        C
        B
        A))
(R
    (Free
        E
        D
        C
        (Free
            A)))
(R
    (Free
        E
        (Free
            C)
        (Free
            A)
(R
    (Free
        F
        E
        (Free
            C)
        (Free
            A)
(R
    (Free
        G
        F
        E
        (Free
            C)
        (Free
            A)

(R
    (Free
        G
        (Free
            E)
        (Free
            C)
        (Free
            A)


*/