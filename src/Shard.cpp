#include "KVik/Shard.h"

void Shard::add(Key, Value) {

}

Value Shard::get(Key) {
    return Value();
}

void Shard::remove(Key) {

}

void Shard::clear() {

}

Shard::Shard(Shard::ptr<MemoryAllocator> indexAlloc, Shard::ptr<MemoryAllocator> logAlloc,
             Shard::ptr<MemoryAllocator> dataAlloc, Shard::ptr<MemoryAllocator> fastDataAlloc) {

}
