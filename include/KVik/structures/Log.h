#ifndef KVIK_LOG_H
#define KVIK_LOG_H

#include <optional>
#include <memory>
#include <unordered_map>

#include "KVik/KeyValue.h"
#include "KVik/memory/MemoryAllocator.h"

class Log {
private:

    std::unique_ptr<DataFile> data;
    std::unordered_map<Key, Offset> map;

public:
    class iterator {
    public:
        iterator& operator++();
        KeyValue operator*();
    };

    explicit Log(std::unique_ptr<MemoryAllocator>);

    iterator begin();
    iterator end();

    std::optional<Value> get(Key);
    void add(Key);
    void remove(Key);
    void clear();

    bool isFull();
};


#endif //KVIK_LOG_H
