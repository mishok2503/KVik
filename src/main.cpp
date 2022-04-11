#include <zstd.h>
#include <lz4.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <cassert>

using namespace std::chrono;

struct timer {
    void record() {
        a = steady_clock::now();
    }

    void stop() {
        b = steady_clock::now();
    }

    [[nodiscard]] auto tell_mils() const {
        return duration_cast<milliseconds>(b - a);
    }

    [[nodiscard]] auto tell_mics() const {
        return duration_cast<microseconds>(b - a);
    }

    [[nodiscard]] auto tell_ns() const {
        return duration_cast<nanoseconds>(b - a);
    }

    time_point<steady_clock> a;
    time_point<steady_clock> b;
};

size_t compress_via_lz4(char *src, char *dst, size_t srcCount, size_t dstCount, int compression_lvl) {
    return LZ4_compress_default(src, dst, (int) srcCount, (int) dstCount);
}

size_t compress_via_zstd(char *src, char *dst, size_t srcCount, size_t dstCount, int compression_lvl) {
    return ZSTD_compress(dst, dstCount, src, srcCount, compression_lvl);
}

size_t decompress_lz4(char *src, char *dst, int srcCount, int dstCount) {
    return LZ4_decompress_safe(src, dst, srcCount, dstCount);
}

size_t decompress_zstd(char *src, char *dst, int srcCount, int dstCount) {
    return ZSTD_decompress(dst, dstCount, src, srcCount);
}

int main(int argc, char ** argv) {
    std::vector<
            std::pair<
                    size_t (*)(char *, char *, size_t, size_t, int),
                    size_t (*) (char *, char *, int, int)
            >
        > compressor_decompressor_pairs = {
            {compress_via_lz4, decompress_lz4},
            {compress_via_zstd, decompress_zstd}
    };
    int compression_lvls[] = {1, 7};

    std::ofstream out_coefs("coefs.txt");
    std::ofstream out_times("times.txt");
    std::ofstream out_speed("speed.txt");

    for (int i = 1; i < argc; ++i) {
        out_coefs << argv[i] << ' ';
        out_times << argv[i] << ' ';
        out_speed << argv[i] << ' ';
        for (auto [compressor, decompressor] : compressor_decompressor_pairs) {
            for (auto lvl : compression_lvls) {
                std::vector<char> data;
                std::string sfilename = "./silesia/";
                sfilename += argv[i];
                std::ifstream file(sfilename);
                if (file.fail()) {
                    throw std::runtime_error("File not found");
                }
                file.seekg(0, std::ios::end);
                size_t sz = file.tellg();
                data = std::vector<char>(sz);
                file.seekg(0);
                file.read(data.data(), sz);
                file.close();

                size_t compressed_data_cap = ZSTD_compressBound(data.size());
                char *compressed_data = new char[compressed_data_cap];
                timer compr_time;
                compr_time.record();
                size_t compressed_size = compressor(data.data(), compressed_data, data.size(), compressed_data_cap, lvl);
                compr_time.stop();

                out_times << compr_time.tell_mics().count() << ' ';
                out_speed << ((double) data.size() * 1e6) /  ((double) compr_time.tell_mics().count() * (double) (1 << 20)) << ' ';
                out_coefs << (double) compressed_size / (double) data.size() * 100 << ' ';

                char *decompressed_data = new char[data.size()];
                size_t decompressed_size = decompressor(compressed_data, decompressed_data, (int) compressed_size, (int) data.size());
                if (decompressed_size != data.size()) {
                    std::cout << "Decompressed size: " << decompressed_size << ' ' << ", whereas compressed size: " << compressed_size << std::endl;
                    assert(false);
                }
                for (int j = 0; j < decompressed_size; ++j) {
                    if (decompressed_data[j] != data[j]) {
                        assert(false);
                    }
                }
                delete[] decompressed_data;
                delete[] compressed_data;
                if (compressor == compress_via_lz4) break;
            }
        }
        out_speed << '\n';
        out_times << '\n';
        out_coefs << '\n';
    }
}

