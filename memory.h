#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

#include "common.h"
#include "node.h"



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
    // when this block is free, this points (unordered) to the next 
    // and previous free blocks
    LNode node;
};

const u32 allocated_block_size = sizeof(Block) - sizeof(LNode);

FORCE_INLINE Block* 
next_block(Block* b) { return b + b->size + sizeof(Block); }

FORCE_INLINE Block*
block_header(void* ptr) { return (Block*)((u8*)ptr - allocated_block_size);  }

FORCE_INLINE Block*
block_from_node(LNode* node) { return (Block*)((u8*)node - offsetof(Block, node)); }

// represents a region of memory, which may not be directly bordering connected regions,
// so we have to store prev and next
struct Region {
    Region* next;
    Region* prev;
    u32     size;
    LNode   free_blocks;
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
move(void* dst, void* src, u32 size);

// copies 'size' bytes of data at 'src' to 'dst' 
void
copy(void* dst, void* src, u32 size);

void
zero(void* dst, u32 size);

void
initialize();

extern Controller controller;

} // namespace memory

#endif // KERNEL_MEMORY_H