#include "KVik/KVikBuilder.h"

#include "KVik/KVikImpl.h"

std::unique_ptr<KVik> KVikBuilder::build() const {
    return std::make_unique<KVikImpl>();
}
