#include <memory>
#include "memory/MallocDRAMMemoryAllocator.hpp"
#include "memory/DRAMMemory.hpp"

std::unique_ptr<Memory> MallocDRAMMemoryAllocator::alloc(Size size) {
    void *buf = malloc(size);
    return std::make_unique<DRAMMemory>(buf, size);
}

void MallocDRAMMemoryAllocator::dealloc(std::unique_ptr<Memory> &&memory) {
    // static cast is faster and in this case we are sure that the same type of memory is deallocated, otherwise it is programmer's mistake
    // NOLINTNEXTLINE
    auto *memoryPtr = static_cast<DRAMMemory *>(memory.get());
    free(memoryPtr->_buf);
    memoryPtr->_bufSize = 0;
    memoryPtr->_buf = nullptr;
}