#include "BiConsumer.h"

namespace java::util::function {
    void BiConsumer::accept(const shared<Object> T, const shared<Object> U) {
        inner(T, U);
    }

    shared<BiConsumer> BiConsumer::andThen(const shared<BiConsumer> after) {
        return alloc<BiConsumer>(lambda(const shared<Object> &T, const shared<Object> &U) {
            self->accept(T, U);
            after->accept(T, U);
        });
    }
}
