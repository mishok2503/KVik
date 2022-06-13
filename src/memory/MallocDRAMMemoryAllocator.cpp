#include "KVik/memory/MallocDRAMMemoryAllocator.h"

#include <memory>

#include "KVik/memory/DRAMMemory.h"

std::unique_ptr<Memory> MallocDRAMMemoryAllocator::alloc(Size size) {
    if (size == 0) {
        return std::make_unique<DRAMMemory>(nullptr, 0);
    }
    void *buf = malloc(size);
    return std::make_unique<DRAMMemory>(buf, size);
}

std::unique_ptr<Memory> MallocDRAMMemoryAllocator::alloc(std::unique_ptr<Memory> &&memory, Size size) {
    if (memory->size() == 0 || size == 0) {
        if (size == 0) {
            dealloc(std::move(memory));
        }
        return alloc(size);
    }
    // static cast is faster and in this case we are sure that the same type of memory is deallocated, otherwise it is programmer's mistake
    // NOLINTNEXTLINE
    auto *memoryPtr = static_cast<DRAMMemory *>(memory.get());
    void *reallocatedBuf = realloc(memoryPtr->_buf, size);
    if (!reallocatedBuf) {
        throw AllocationException("error while reallocating in MallocFRAMMemoryAllocator");
    }
    return std::make_unique<DRAMMemory>(reallocatedBuf, size);
}

void MallocDRAMMemoryAllocator::dealloc(std::unique_ptr<Memory> &&memory) {
    // static cast is faster and in this case we are sure that the same type of memory is deallocated, otherwise it is programmer's mistake
    // NOLINTNEXTLINE
    auto *memoryPtr = static_cast<DRAMMemory *>(memory.get());
    free(memoryPtr->_buf);
    memoryPtr->_bufSize = 0;
    memoryPtr->_buf = nullptr;
}