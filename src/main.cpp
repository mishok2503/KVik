#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <string>

#include "MurmurHash3.h"
#include "SpeedTest.h"
#include "xxhash.h"

struct HashFunc {
    pfHash func;
    std::string name;
};

char memcpy_buf[2048];

int main(int argc, char **argv) {
    if (argc != 2 && argc != 3) {
        std::cerr << "Usage: " << argv[0] << " output-file-prefix [0/1 need log output]\n";
        return 1;
    }
    bool verbose = true;
    if (argc == 3 && std::string(argv[2]) == "0") {
        verbose = false;
    }

    std::vector<HashFunc> hash_funcs = {
        {MurmurHash3_x64_128, "MurMurHash"},
        {[](const void *key, int len, uint32_t seed, void *out) {
            auto hash = XXH3_128bits_withSeed(key, len, seed);
            *(uint64_t *) out = hash.high64;
            *((uint64_t *) out + 1) = hash.high64;
        }, "xxHash"},
        {[](const void *key, int len, uint32_t, void *out) {
            memcpy(memcpy_buf, key, len);
            out = memcpy_buf;
        }, "memcpy"}
    };

    auto test = [&](int min_size, int max_size, int step, const std::string& file_suffix) {
        constexpr int seed = 239;
        int unused;
        double res;
        std::string file_name = argv[1] + ('_' + file_suffix + ".txt");
        std::ofstream out(file_name);
        if (!out.is_open()) {
            std::cerr << "Can't open file " << file_name << '\n';
            return;
        }
        
        for (auto func : hash_funcs) {
            out << func.name << '\n';
            for (int key_size = min_size; key_size <= max_size; key_size += step) {
                if (verbose) {
                    std::cout << func.name << '(' << file_suffix << "): ";
                    std::cout.flush();
                }
                TinySpeedTest(func.func, unused, key_size, seed, verbose, res);
                out << '\t' << key_size << " : " << res << '\n';
            }
            out << '\n';
        }
        out.close();
    };

    test(1, 100, 1, "small");
    test(100, 1000, 50, "big");
    
    return 0;
}
