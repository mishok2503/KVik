project(HashBenchmarks)

add_subdirectory(./smhasher/src)
add_subdirectory(./xxHash/cmake_unofficial)

add_executable(benchmarks src/main.cpp)
include_directories(./smhasher/src)
include_directories(./xxHash)

target_link_libraries(benchmarks PRIVATE SMHasherSupport)
target_link_libraries(benchmarks PRIVATE xxHash::xxhash)