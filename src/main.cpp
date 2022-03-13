#include <iostream>
#include <cstring>

#include "../smhasher/src/MurmurHash3.h"

int main(int argc, char **argv) {
    __uint128_t out;
    MurmurHash3_x86_128(argv[1], strlen(argv[1]), 239, &out);
    std::cout << ((uint64_t) (out >> 64)) << ' ' << (uint64_t) out << std::endl;
    return 0;
}
