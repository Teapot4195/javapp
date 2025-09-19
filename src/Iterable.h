#pragma once

#include "Object.h"

namespace java::util {
    class Iterator;
    class Spliterator;

    namespace function {
        class Consumer;
    }
}

class Iterable : public virtual Object {
public:
    ~Iterable() override = default;

    virtual void forEach(shared<java::util::function::Consumer> action);

    virtual shared<java::util::Iterator> iterator() = 0;

    virtual shared<java::util::Spliterator> spliterator();
};
