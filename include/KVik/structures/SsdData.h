#ifndef KVIK_SSD_DATA_H
#define KVIK_SSD_DATA_H

#include "KVik/structures/HotFiles.h"

#include "KVik/memory/ZeroedMemoryAllocator.h"
#include "KVik/memory/Memory.h"

static constexpr Offset SSD_DATA_HEADER_SIZE = sizeof(Offset);
static constexpr Offset SSD_DATA_MAX_RECORDS = 10'000;
static constexpr Offset SSD_DATA_FILE_SIZE = SSD_DATA_HEADER_SIZE + sizeof(KeyValue) * SSD_DATA_MAX_RECORDS;

struct SsdData : public HotFiles {
public:
    explicit SsdData(std::unique_ptr<ZeroedMemoryAllocator> &&allocator);

    Offset add(KeyValue const &keyValue) override;
    void remove(Offset offset) override;
    std::optional<Value> get(Offset) override;
    Size size() override;

private:
    Offset fetchSize();

    std::unique_ptr<ZeroedMemoryAllocator> _allocator;
    std::unique_ptr<Memory> _memory;
};

#endif //KVIK_SSD_DATA_H