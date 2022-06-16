#include "KVik/structures/SsdData.h"

#include <optional>

#include "KVik/KeyValue.h"

SsdData::SsdData(std::unique_ptr<ZeroedMemoryAllocator> &&allocator) : _allocator(std::move(allocator)), _memory(_allocator->alloc(SSD_DATA_FILE_SIZE)) {}

Offset SsdData::add(KeyValue const &keyValue) {
    Offset curSize = fetchSize();
    Offset endPtr = SSD_DATA_HEADER_SIZE + curSize * sizeof(KeyValue);
    _memory->write(endPtr, sizeof (KeyValue), &keyValue);
    return endPtr;
}

void SsdData::remove(Offset offset) {

}

std::optional<Value> SsdData::get(Offset) {
    return {};
}

Offset SsdData::fetchSize() {
    Offset size;
    _memory->read(0, sizeof (Offset), &size);
    return size;
}
