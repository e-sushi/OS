#include "common.h"

namespace memory {

struct Region;

// a contiguous block of memory in a Region
struct Block {
    Block* prev;
    Region* region; // pointer to the region this block is contained in 
    struct {
        u32 allocated : 1; // this Block is allocated and cannot be merged
        u32 size : 31; // bytes
    };
    Block* next_free; // if this is allocated, then this is the start of allocated data, otherwise it points to the next free block
};

// represents a region of memory, which may not be directly bordering connected regions,
// so we have to store prev and next
struct Region {
    Region* next;
    Region* prev;
    u32     size;
    Block* first_free_block;
};

struct Controller {
    Region* region;
};

void* 
allocate(u32 size);

template<typename T> T*
allocate();

template<typename T> T* 
allocate(u32 count);

void 
free(void* ptr);

void
initialize();

FORCE_INLINE Block*
block_from_addr(void* addr);

extern Controller controller;

} // namespace memory