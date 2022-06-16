#ifndef KVIK_KEYVALUE_H
#define KVIK_KEYVALUE_H

#include <cstddef>
#include <KVik/Constants.h>
#include <utility>

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
