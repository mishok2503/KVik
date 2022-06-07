#include "KVik/KVikImpl.h"

std::future<void> KVikImpl::remove(Key) {
    return std::future<void>();
}

std::future<void> KVikImpl::add(Key, Value) {
    return std::future<void>();
}

std::future<Value> KVikImpl::get(Key) {
    std::promise<Value> p;
    p.set_value({'a', 'b', 'c'});
    return p.get_future();
}

std::future<void> KVikImpl::clear() {
    return std::future<void>();
}
