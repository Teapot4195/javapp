#include "Iterator.h"

#include "function/Consumer.h"

namespace java::util {
    void Iterator::forEachRemaining(shared<function::Consumer> action) {
        while (hasNext())
            action->accept(next());
    }

    void Iterator::remove() {
        throw std::runtime_error("TODO: THROW UNSUPPORTED OPERATION EXCEPTION");
    }
}
