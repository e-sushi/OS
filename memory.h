#include "common.h"

namespace memory {

struct Block {
    Block* next;
    u32    size; // bytes
};

struct Controller {
    Block* free_blocks;

};

void* 
allocate(u32 size);

template<typename T> T* 
allocate(u32 count = 1);

void 
free(void* ptr);

void
initialize();

extern Controller controller;

} // namespace memory