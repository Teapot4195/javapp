#include "DoubleConsumer.h"

namespace java::util::function {
    void DoubleConsumer::accept(const double value) {
        inner(value);
    }

    shared<DoubleConsumer> DoubleConsumer::andThen(const shared<DoubleConsumer> after) {
        return alloc<DoubleConsumer>(lambda(const double value) {
            self->accept(value);
            after->accept(value);
        });
    }
}
