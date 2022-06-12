#include "KVik/memory/MmapDRAMMemoryAllocator.h"

#include <memory>
#include <cerrno>
#include <sys/mman.h>

#include "KVik/memory/Memory.h"
#include "KVik/memory/DRAMMemory.h"

std::unique_ptr<Memory> MmapDRAMMemoryAllocator::alloc(Size size) {
    void *buf = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (buf == MAP_FAILED) {
        throw MemoryException(std::string("error during MmapDRAMMemoryAllocator::alloc, MAP_FAILED, errno is ") + std::to_string(errno));
    }
    return std::make_unique<DRAMMemory>(buf, size);
}