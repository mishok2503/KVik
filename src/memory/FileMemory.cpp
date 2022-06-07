#include "memory/FileMemory.hpp"


/* FixedFileMemory member functions definitions */
FixedFileMemory::FixedFileMemory(
        FILE *file,
        Size size,
        std::string filename
) : _file(file), _size(size), _filename(std::move(filename)) {}

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
    return _size;
}


/* ExtendableFileMemory member functions definitions */
ExtendableFileMemory::ExtendableFileMemory(
        FILE *file,
        Size size,
        std::string filename
) : FixedFileMemory(file, size, std::move(filename)) {}

void ExtendableFileMemory::write(Offset offset, Size count, void *data) {
    if (offset + count > _size) {
        if (ftruncate(fileno(_file), offset + count) < 0) {
            throw MemoryException("error during ftruncate");
        }
        _size = offset + count;
    }
    FixedFileMemory::write(offset, count, data);
}