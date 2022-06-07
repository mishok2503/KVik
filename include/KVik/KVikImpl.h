#ifndef KVIK_KVIKIMPL_H
#define KVIK_KVIKIMPL_H

#include "KVik/KVik.h"

class KVikImpl : public KVik {
private:

public:
    std::future<void> remove(Key) override;
    std::future<void> add(Key, Value) override;
    std::future<Value> get(Key) override;
    std::future<void> clear() override;
};

#endif //KVIK_KVIKIMPL_H
