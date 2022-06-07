#include "memory/DRAMMemory.h"

void DRAMMemory::checkRange(Offset offset, Size count) const {
    if (offset + count > _bufSize) {
        throw MemoryException(
                "error while working with DRAM memory, attempt to access memory that is out of bounds");
    }
}

DRAMMemory::DRAMMemory(void *buf, Size bufSize) : _buf(buf), _bufSize(bufSize) {}

void DRAMMemory::write(Offset offset, Size count, void *data) {
    checkRange(offset, count);
    std::memcpy(_buf, data, count);
}

void DRAMMemory::read(Offset offset, Size count, void *data) {
    checkRange(offset, count);
    std::memcpy(data, _buf, count);
}

Size DRAMMemory::size() noexcept {
    return _bufSize;
}
