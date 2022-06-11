#ifndef KVIK_KEYVALUE_H
#define KVIK_KEYVALUE_H

#include <cstddef>
#include <utility>

constexpr size_t KEY_SIZE = 16;
constexpr size_t VALUE_SIZE = 2048;

struct Key {
    char data[KEY_SIZE];
};

struct Value {
    char data[VALUE_SIZE];
};

using KeyValue = std::pair<Key, Value>;

bool operator==(Key const&, Key const&);
bool operator==(Value const&, Value const&);

#endif //KVIK_KEYVALUE_H
