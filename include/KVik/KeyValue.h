#ifndef KVIK_KEYVALUE_H
#define KVIK_KEYVALUE_H

#include <cstddef>

constexpr size_t KEY_SIZE = 8;
constexpr size_t VALUE_SIZE = 2048;

struct Key {
    char data[KEY_SIZE];
};

struct Value {
    char data[VALUE_SIZE];
};

#endif //KVIK_KEYVALUE_H
