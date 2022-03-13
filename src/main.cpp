#include <iostream>
#include <cstring>

#include "../smhasher/src/MurmurHash3.h"
#include "../smhasher/src/SpeedTest.h"

int main(int argc, char **argv) {
    constexpr int seed = 239;
    double unused;
    for (int i = 20; i <= 1e4; i += 200) {
        TinySpeedTest(MurmurHash3_x64_128, 42, i, seed, true, unused);
    }
    return 0;
}
