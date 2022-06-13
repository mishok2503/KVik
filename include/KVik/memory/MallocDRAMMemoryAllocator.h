#ifndef KVIK_MALLOC_DRAM_MEMORY_ALLOCATOR_H
#define KVIK_MALLOC_DRAM_MEMORY_ALLOCATOR_H

#include "MemoryAllocator.h"

struct MallocDRAMMemoryAllocator : MemoryAllocator {

    std::unique_ptr<Memory> alloc(Size size) override;

    std::unique_ptr<Memory> alloc(std::unique_ptr<Memory> &&memory, Size size) override;

    void dealloc(std::unique_ptr<Memory> &&memory) override;
};

#endif