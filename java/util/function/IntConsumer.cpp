#include "IntConsumer.h"

#include "java/util/stream/LongStream.h"

namespace java::util::function {
    void IntConsumer::accept(const int value) {
        inner(value);
    }

    shared<IntConsumer> IntConsumer::andThen(const shared<IntConsumer> after) {
        if (after == nullptr)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION");
        return alloc<IntConsumer>([&](const int value) {
            accept(value);
            after->accept(value);
        });
    }
}
