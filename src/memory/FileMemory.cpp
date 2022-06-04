#include "FileMemory.hpp"


/* FixedFileMemory member functions definitions */
FixedFileMemory::FixedFileMemory(FILE *file) : _file(file) {}

void FixedFileMemory::changeFilePosition(Offset offset) {
    if (std::fseek(_file, offset, SEEK_SET) < 0) {
        throw MemoryException(std::string("error during fseek, errno is ") + std::to_string(errno));
    }
}

void FixedFileMemory::write(Offset offset, Size count, void *data) {
    changeFilePosition(offset);
    if (std::fwrite(data, 1, count, _file) != count) {
        throw MemoryException("error during std::fwrite");
    }
}

void FixedFileMemory::read(Offset offset, Size count, void *data) {
    changeFilePosition(offset);
    if (std::fread(data, 1, count, _file) != count) {
        throw MemoryException("error during std::fread");
    }
}

Size FixedFileMemory::size() {
    std::fseek(_file, 0L, SEEK_END);
    return std::ftell(_file);
}


/* ExtendableFileMemory member functions definitions */
ExtendableFileMemory::ExtendableFileMemory(FILE *file) : FixedFileMemory(file) {}

void ExtendableFileMemory::write(Offset offset, Size count, void *data) {
    if (offset + count > FixedFileMemory::size()) {
        if (ftruncate(fileno(_file), offset + count) < 0) {
            throw MemoryException("error during ftruncate");
        }
    }
    FixedFileMemory::write(offset, count, data);
}