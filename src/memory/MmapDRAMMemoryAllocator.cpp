#include "KVik/memory/MmapDRAMMemoryAllocator.h"

#define _GNU_SOURCE

#include <memory>
#include <cerrno>
#include <sys/mman.h>

#include "KVik/memory/Memory.h"
#include "KVik/memory/DRAMMemory.h"

std::unique_ptr<Memory> MmapDRAMMemoryAllocator::alloc(Size size) {
    if (size == 0) {
        return std::make_unique<DRAMMemory>(nullptr, 0);
    }
    void *buf = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (buf == MAP_FAILED) {
        throw MemoryException(std::string("error during MmapDRAMMemoryAllocator::alloc, MAP_FAILED, errno is ") + std::to_string(errno));
    }
    return std::make_unique<DRAMMemory>(buf, size);
}

std::unique_ptr<Memory> MmapDRAMMemoryAllocator::alloc(std::unique_ptr<Memory> &&memory, Size size) {
    if (size == 0 || memory->size() == 0) {
        return alloc(size);
    }
    // NOLINTNEXTLINE
    auto *memoryPtr = static_cast<DRAMMemory *>(memory.get());
    void *buf = mremap(memoryPtr->_buf, memoryPtr->_bufSize, size, MREMAP_MAYMOVE);
    if (buf == MAP_FAILED) {
        throw MemoryException(std::string("error during MmapDRAMMemoryAllocator::alloc, MAP_FAILED, errno is ") + std::to_string(errno));
    }
    return std::make_unique<DRAMMemory>(buf, size);
}

void MmapDRAMMemoryAllocator::dealloc(std::unique_ptr<Memory> &&memory) {
    // NOLINTNEXTLINE
    auto *memoryPtr = static_cast<DRAMMemory *>(memory.get());
    // TODO: handle munmap error
    munmap(memoryPtr->_buf, memoryPtr->_bufSize);
    memoryPtr->_bufSize = 0;
    memoryPtr->_buf = nullptr;
}