#include "DoubleConsumer.h"

namespace java::util::function {
    void DoubleConsumer::accept(const double value) {
        inner(value);
    }

    shared<DoubleConsumer> DoubleConsumer::andThen(const shared<DoubleConsumer> after) {
        return alloc<DoubleConsumer>([=](const double value) {
            accept(value);
            after->accept(value);
        });
    }
}
