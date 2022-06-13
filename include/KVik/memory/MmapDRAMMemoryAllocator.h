#include "KVik/memory/ZeroedMemoryAllocator.h"

struct MmapDRAMMemoryAllocator : ZeroedMemoryAllocator {
public:
    std::unique_ptr<Memory> alloc(Size size) override;

    std::unique_ptr<Memory> alloc(std::unique_ptr<Memory> &&memory, Size size) override;

    void dealloc(std::unique_ptr<Memory> &&memory) override;
};