#ifndef KVIK_KVIKBUILDER_H
#define KVIK_KVIKBUILDER_H

#include <memory>

#include "KVik.h"

class KVikBuilder {


public:
    std::unique_ptr<KVik> build() const;
};


#endif //KVIK_KVIKBUILDER_H
