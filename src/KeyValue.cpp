#include "KVik/KeyValue.h"

bool operator==(Key const &lhs, Key const &rhs) {
    for (int i = 0; i < KEY_SIZE; ++i) {
        if (lhs.data[i] != rhs.data[i]) {
            return false;
        }
    }
    return true;
}

bool operator==(Value const &lhs, Value const &rhs) {
    for (int i = 0; i < VALUE_SIZE; ++i) {
        if (lhs.data[i] != rhs.data[i]) {
            return false;
        }
    }
    return true;
}