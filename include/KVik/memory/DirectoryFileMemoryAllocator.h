#ifndef KVIK_DIRECTORY_FILE_MEMORY_ALLOCATOR_H
#define KVIK_DIRECTORY_FILE_MEMORY_ALLOCATOR_H

#include <memory>
#include <cstdio>
#include <fcntl.h>
#include "KVik/memory/FileMemory.h"
#include "KVik/memory/MemoryAllocator.h"
#include "KVik/memory/ZeroedMemoryAllocator.h"

std::string generateRandomFilename(const std::string& prefix);

template<typename FileMemoryT>
struct DirectoryFileMemoryAllocator : ZeroedMemoryAllocator {

    explicit DirectoryFileMemoryAllocator(
            std::string dirname,
            std::string filenamePrefix
    ) : _filenamePrefix(std::move(filenamePrefix)), _dirname(std::move(dirname)) {
        _dirfd = open(_dirname.c_str(), O_DIRECTORY);
        if (_dirfd < 0) {
            throw MemoryException(
                    "error while constructing DirectoryFileMemoryAllocator, can't open directory passed to constructor");
        }
    }

    std::unique_ptr<Memory> alloc(Size size) override {
        std::string nextFilename;
        int fd;
        do {
            nextFilename = generateRandomFilename(_filenamePrefix);
        } while (
                (fd = openat(_dirfd, nextFilename.c_str(), O_CREAT | O_EXCL | O_TRUNC | O_RDWR, 00666)) < 0
                && errno == EEXIST
                );
        if (fd < 0) {
            throw MemoryException(
                    "error in DirectoryFileMemoryAllocator::alloc, can't open file for reading and writing");
        }
        FILE *file = fdopen(fd, "w+");
        if (!file) {
            throw MemoryException(
                    "error in DirectoryFileMemoryAllocator::alloc, can't open file for reading and writing");
        }
        if (ftruncate(fileno(file), size) < 0) {
            throw AllocationException("couldn't allocate file of given size in this directory");
        }
        return std::make_unique<FileMemoryT>(file, size, nextFilename);
    }

    std::unique_ptr<Memory> alloc(std::unique_ptr<Memory> &&memory, Size size) override {
        // static cast is faster and in this case we are sure that the same type of memory is deallocated, otherwise it is programmer's mistake
        // NOLINTNEXTLINE
        auto *memoryPtr = static_cast<FileMemoryT *>(memory.get());
        if (ftruncate(fileno(memoryPtr->_file), size) < 0) {
            throw AllocationException("error while attempting to reallocate file using ftruncate");
        }
        memoryPtr->_size = size;
        return std::unique_ptr<Memory>(memoryPtr);
    }

    void dealloc(std::unique_ptr<Memory> &&memory) override {
        // static cast is faster and in this case we are sure that the same type of memory is deallocated, otherwise it is programmer's mistake
        // NOLINTNEXTLINE
        auto *memoryPtr = static_cast<FileMemoryT *>(memory.get());
        fclose(memoryPtr->_file);
        std::string pathToRemove = _dirname + "/" + memoryPtr->_filename;
        std::remove(pathToRemove.c_str());
        memoryPtr->_size = 0;
        memoryPtr->_file = nullptr;
    }

private:

    int _dirfd;

    std::string _filenamePrefix;

    std::string _dirname;

};

#endif