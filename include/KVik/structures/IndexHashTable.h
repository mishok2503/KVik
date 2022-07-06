#ifndef KVIK_INDEXHASHTABLE_H
#define KVIK_INDEXHASHTABLE_H

#include <cstdint>
#include "KVik/KeyValue.h"
#include "KVik/memory/FileMemory.h"
#include "KVik/memory/ZeroedMemoryAllocator.h"

class IndexHashTable {
private:
    size_t _bucketCnt;
    std::unique_ptr<Memory> _memory;
    std::unique_ptr<ZeroedMemoryAllocator> _allocator;
    char _buffer[BUCKET_SIZE]{};

    void resize();

public:
    explicit IndexHashTable(std::unique_ptr<ZeroedMemoryAllocator> &&allocatorPtr);

    // WARNING: it is extremely important that actual memory is compatible with allocator that is used for
    // memory deallocation (for instance, if MmapDRAMMemoryAllocator is used, then dumpedIndexHT should be DRAMMemory,
    // plus it should have been allocated via mmap)
    explicit IndexHashTable(std::unique_ptr<ZeroedMemoryAllocator> &&allocatorPtr, std::unique_ptr<Memory> &&dumpedIndexHT);

    void put(const Key &key, int64_t value);

    int64_t get(Key const &key);

    void remove(Key const &key);
};

#endif //KVIK_INDEXHASHTABLE_H
