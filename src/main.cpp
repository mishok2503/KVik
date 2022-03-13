#include <iostream>
#include <string>

#include "MurmurHash3.h"

int main(int argc, char **argv) {

    std::string str = argv[1];
    uint32_t out;

    MurmurHash3_x64_128(str.data(), str.size(), 239, &out);

    std::cout << out << std::endl;

    return 0;
}
