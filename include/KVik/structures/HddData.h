#include "KVik/structures/DataFile.h"

#include <memory>

#include "KVik/memory/MemoryAllocator.h"

static constexpr uint64_t HDD_DATA_OFF = sizeof (Offset) + sizeof(Size);

// memory layout of _memory is going to be like this: [head|VALUE|VALUE|VALUE|next_|VALUE|next_|...]
class HddData : public DataFile {
private:

    std::unique_ptr<MemoryAllocator> _allocator;

    std::unique_ptr<Memory> _memory;

public:

    explicit HddData(std::unique_ptr<MemoryAllocator> &&allocator);

    void remove(Offset offset) override;

    std::vector<Offset> write(std::unique_ptr<Memory> &memory, const std::vector<Offset> &offsets) override;

    Offset write(std::unique_ptr<Memory> &memory, Offset offset) override;

    Offset write(Value) override;

    Value read(Offset offset) override;

    Size size() override;

    ~HddData();

private:

    Offset fetchHead();

    Offset getNext(Offset head);

    void storeHead(Offset head);

    Size fetchSize();

    void storeSize(Size size);

    void increaseSize();

    void decreaseSize();

};