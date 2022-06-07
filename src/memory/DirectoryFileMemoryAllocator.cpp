#include <random>
#include <string>
#include "memory/DirectoryFileMemoryAllocator.h"

std::string generateRandomFilename(const std::string& prefix) {
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_int_distribution<std::uint64_t> dist(0, UINT64_MAX);
    return prefix + std::to_string(dist(rng));
}