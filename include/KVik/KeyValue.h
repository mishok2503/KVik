#ifndef KVIK_KEYVALUE_H
#define KVIK_KEYVALUE_H

#include <cstddef>
#include <KVik/Constants.h>

struct Key {
    char data[KEY_SIZE];
};

struct Value {
    char data[VALUE_SIZE];
};

#endif //KVIK_KEYVALUE_H
