project(HashBenchmarks)

add_subdirectory(./smhasher/src)

add_executable(benchmarks ./src/main.cpp)
include_directories(./smhasher/src)

target_link_libraries(benchmarks SMHasherSupport)