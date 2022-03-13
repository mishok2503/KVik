#include <iostream>
#include <string>
#include <cstring>

#include "MurmurHash3.h"

int main(int argc, char **argv) {
    uint32_t out;
    MurmurHash3_x64_128(argv[1], strlen(argv[1]), 239, &out);
    std::cout << out << std::endl;
    return 0;
}
