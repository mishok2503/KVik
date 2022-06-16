#ifndef KVIK_CONSTANTS_H
#define KVIK_CONSTANTS_H
#include <cstddef>
#include <cinttypes>

constexpr size_t KEY_SIZE = 16;
constexpr size_t VALUE_SIZE = 2048;
constexpr size_t OFFSET_SIZE = sizeof(int64_t);
constexpr uint64_t BUCKET_SIZE = 4096;
constexpr int PRIME_SIZES_COUNT = 30;

constexpr uint64_t PRIME_SIZES[PRIME_SIZES_COUNT] = {
        7, 17, 31, 61, 127, 241, 487, 967, 1'931, 3'847, 7'681, 15'361,
        30'727, 61'441, 122'887, 245'771, 491'527, 983'063, 1'996'081, 3'932'167,
        7'864'331, 15'728'681, 31'457'287, 60'000'011, 120'000'007, 240'000'011, 480'000'019,
        960'000'011, 1'900'000'043, 3'800'000'003,
};

#endif //KVIK_CONSTANTS_H
