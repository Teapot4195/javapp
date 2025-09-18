#include "LongConsumer.h"

namespace java::util::function {
    void LongConsumer::accept(long long value) {
        inner(value);
    }

    shared<LongConsumer> LongConsumer::andThen(const shared<LongConsumer> after) {
        return alloc<LongConsumer>(lambda(const long long value) {
            self->accept(value);
            after->accept(value);
        });
    }
}
