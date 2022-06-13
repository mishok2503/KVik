#ifndef KVIK_MEMORY_ALLOCATOR_H
#define KVIK_MEMORY_ALLOCATOR_H

#include <memory>
#include <stdexcept>

#include "Memory.h"

struct AllocationException : std::runtime_error {

    using std::runtime_error::runtime_error;

};

struct MemoryAllocator {
    virtual std::unique_ptr<Memory> alloc(Size size) = 0;

    virtual std::unique_ptr<Memory> alloc(std::unique_ptr<Memory> &&memory, Size size) = 0;

    virtual void dealloc(std::unique_ptr<Memory> &&memory) = 0;

    virtual ~MemoryAllocator() = default;
};

#endif