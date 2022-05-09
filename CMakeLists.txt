cmake_minimum_required(VERSION 3.16)
project(HashBenchmarks)
set(CMAKE_CXX_STANDARD 17)

include_directories(smhasher/src)
include_directories(xxHash)
add_subdirectory(smhasher/src)
add_subdirectory(xxHash/cmake_unofficial)

add_executable(benchmarks src/main.cpp)

add_executable(experiments src/experiment.cpp)

target_link_libraries(benchmarks PRIVATE SMHasherSupport)
target_link_libraries(experiments PRIVATE SMHasherSupport)
target_link_libraries(benchmarks PRIVATE xxHash::xxhash)
target_link_libraries(experiments PRIVATE xxHash::xxhash)
