#include "KVik/structures/HddData.h"

#include <memory>

#include "KVik/memory/MemoryAllocator.h"

HddData::HddData(std::unique_ptr<MemoryAllocator> &&allocator)
: _allocator(std::move(allocator)), _memory(std::move(_allocator->alloc(HDD_DATA_OFF))) {
    storeSize(0);
    storeHead(0);
}

void HddData::remove(Offset offset) {
    Offset head = fetchHead();
    // set next of out current deleted block as previous head
    _memory->write(offset, sizeof (Offset), &head);
    // make offset new head of list
    _memory->write(0, sizeof (Offset), &offset);
    decreaseSize();
}

Offset HddData::fetchHead() {
    Offset head;
    // fetch head from the memory
    _memory->read(0, sizeof (Offset), &head);
    return head;
}

Offset HddData::getNext(Offset head) {
    Offset next;
    _memory->read(head, sizeof(Offset), &next);
    return next;
}

void HddData::storeHead(Offset head) {
    _memory->write(0, sizeof (Offset), &head);
}

Size HddData::fetchSize() {
    Size size;
    _memory->read(sizeof (Offset), sizeof(Size), &size);
    return size;
}

void HddData::storeSize(Size size) {
    _memory->write(sizeof (Offset), sizeof(Size), &size);
}

void HddData::increaseSize() {
    Size size = fetchSize();
    storeSize(size + 1);
}

void HddData::decreaseSize() {
    Size size = fetchSize();
    storeSize(size - 1);
}

// TODO : for security add journal mechanism
std::vector<Offset> HddData::write(std::unique_ptr<Memory> &memory, const std::vector<Offset> &offsets) {
    Offset head = fetchHead();
    std::vector<Offset> resOffsets;
    char buf[sizeof(Value) * 4];
    for (int i = 0; i < offsets.size();) {
        memory->read(offsets[i], sizeof(Value), buf);
        if (head == 0) {
            int j = 1;
            for (; j < 4 && i + j < offsets.size(); ++j) {
                memory->read(offsets[i + j], sizeof (Value), buf + j * sizeof(Value));
            }
            // we expect it to be Extendable memory
            // TODO : remove ALL allocation logic from memory, extendable memory should be deprecated
            resOffsets.push_back(_memory->size());
            _memory->write(_memory->size(), sizeof (Value) * j, buf);
            for (j = 1; j < 4 && i + j < offsets.size(); ++j) {
                resOffsets.push_back(resOffsets.back() + sizeof(Value));
            }
            i += j;
        } else {
            Offset next = getNext(head);
            _memory->write(head, sizeof (Value), buf);
            resOffsets.push_back(head);
            head = next;
        }
    }
    storeHead(head);
    Size size = fetchSize();
    storeSize(size + offsets.size());
    return resOffsets;
}

Offset HddData::write(std::unique_ptr<Memory> &memory, Offset offset) {
    std::vector<Offset> offsets;
    offsets.push_back(offset);
    return write(memory, offsets).back();
}

Value HddData::read(Offset offset) {
    Value value = {};
    _memory->read(offset, sizeof(Value), &value);
    return value;
}

Size HddData::size() {
    return fetchSize();
}

HddData::~HddData() {
    _allocator->dealloc(std::move(_memory));
}

Offset HddData::write(Value value) {
    Offset head = fetchHead();
    Offset resOffset;

    if (head == 0) {
        resOffset = _memory->size();
        _memory->write(_memory->size(), sizeof (Value) , &value);
    } else {
        resOffset = head;
        Offset next = getNext(head);
        _memory->write(head, sizeof (Value), &value);
        head = next;
    }
    increaseSize();
    storeHead(head);
    return resOffset;
}

