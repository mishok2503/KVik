#ifndef KVIK_MEMORY_ALLOCATOR_H
#define KVIK_MEMORY_ALLOCATOR_H

#include <memory>
#include "Memory.h"

struct MemoryAllocator {
    virtual std::unique_ptr<Memory> alloc(Size size) = 0;

    virtual void dealloc(std::unique_ptr<Memory> &&memory) = 0;
};

#endif