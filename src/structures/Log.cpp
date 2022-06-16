#include "KVik/structures/Log.h"

#include <utility>

#include "KVik/structures/HddData.h"

Log::iterator &Log::iterator::operator++() {
    return *this;
}

KeyValue Log::iterator::operator*() {
    return KeyValue();
}

Log::Log(std::unique_ptr<MemoryAllocator> alloc) : data(new HddData(std::move(alloc))) {

}

Log::iterator Log::begin() {
    return Log::iterator();
}

Log::iterator Log::end() {
    return Log::iterator();
}

std::optional<Value> Log::get(Key) {
    return std::optional<Value>();
}

void Log::add(Key) {

}

void Log::remove(Key) {

}

void Log::clear() {

}

bool Log::isFull() {
    return false;
}
