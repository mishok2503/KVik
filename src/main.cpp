#include <iostream>
#include <cstring>

#include "../smhasher/src/MurmurHash3.h"
#include "../smhasher/src/SpeedTest.h"
#include "../xxHash/xxhash.h"

void xxHashWrapper(const void *key, const int len, const uint32_t seed, void *out) {
    auto hash = XXH3_128bits_withSeed(key, len, seed);
    *(uint64_t *) out = hash.high64;
    *((uint64_t *) out + 1) = hash.high64;
}

int main(int argc, char **argv) {
    constexpr int seed = 239;
    double unused;
    std::cout << "Murmur test" << std::endl;
    for (int i = 20; i <= 1e4; i += 200) {
        TinySpeedTest(MurmurHash3_x64_128, 42, i, seed, true, unused);
    }
    std::cout << "XXHash test" << std::endl;
    for (int i = 20; i <= 1e4; i += 200) {
        TinySpeedTest(xxHashWrapper, 42, i, seed, true, unused);
    }
    std::cout << "Huge murmur test" << std::endl;
    TinySpeedTest(MurmurHash3_x64_128, 42, 1000000, seed, true, unused);
    std::cout << "Huge xxhash test" << std::endl;
    TinySpeedTest(xxHashWrapper, 42, 1000000, seed, true, unused);
    return 0;
}
