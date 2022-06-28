#include "KVik/structures/SsdData.h"

#include <optional>

#include "KVik/KeyValue.h"

SsdData::SsdData(std::unique_ptr<ZeroedMemoryAllocator> &&allocator) : _allocator(std::move(allocator)), _memory(_allocator->alloc(SSD_DATA_FILE_SIZE)) {}

Offset SsdData::add(KeyValue const &keyValue) {
    Offset curSize = fetchSize();
    Offset endPtr = SSD_DATA_HEADER_SIZE + curSize * sizeof(KeyValue);
    _memory->write(endPtr, sizeof (KeyValue), &keyValue);

    if (endPtr + sizeof (KeyValue) == SSD_DATA_FILE_SIZE) {
        // merge with DataFile should be invoked
    }

    return endPtr;
}

void SsdData::remove(Offset offset) {
    char zeroes[sizeof(KeyValue)] = {};
    // just mark it zero, in assumption that key is never zero
    _memory->write(offset, sizeof (KeyValue), zeroes);
}

std::optional<Value> SsdData::get(Offset offset) {
    KeyValue kv;
    std::optional<Value> optValue;
    _memory->read(offset, sizeof (KeyValue), &kv);
    if (kv.first == Key{}) {
        return optValue;
    }
    optValue.emplace(kv.second);
    return optValue;
}

Size SsdData::size() {
    return fetchSize();
}

Offset SsdData::fetchSize() {
    Offset size;
    _memory->read(0, sizeof (Offset), &size);
    return size;
}
