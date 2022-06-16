#ifndef KVIK_SHARD_H
#define KVIK_SHARD_H

#include <memory>

#include "KVik/structures/Filter.h"
#include "KVik/structures/Index.h"
#include "KVik/structures/DataFile.h"
#include "KVik/structures/Log.h"
#include "KVik/KeyValue.h"
#include "KVik/memory/MemoryAllocator.h"

class Shard {
private:
    template<typename T>
    using ptr = std::unique_ptr<T>;

    ptr<Filter<Key, Index>> filter;
    ptr<Index> index;
    ptr<Log> log;
    ptr<DataFile> data;
    ptr<DataFile> fastData;

public:
    Shard(ptr<MemoryAllocator> indexAlloc, ptr<MemoryAllocator> logAlloc,
          ptr<MemoryAllocator> dataAlloc, ptr<MemoryAllocator> fastDataAlloc);

    void add(Key, Value);
    Value get(Key);
    void remove(Key);
    void clear();
};


#endif //KVIK_SHARD_H
