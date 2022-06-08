#ifndef KVIK_FILTER_H
#define KVIK_FILTER_H

#include <stdexcept>

struct FilterException : std::runtime_error {

    using std::runtime_error::runtime_error;

};

template<typename K, typename Iterable>
class Filter {
public:

    virtual void add(K const &) = 0;

    virtual bool check(K const &) = 0;

    virtual size_t filled() = 0;

    virtual size_t size() = 0;

    virtual void rebuildFrom(Iterable const &iterator) = 0;

};

#endif //KVIK_FILTER_H
