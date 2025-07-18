#include "BiConsumer.h"

namespace java::util::function {
    void BiConsumer::accept(const shared<Object> T, const shared<Object> U) {
        inner(T, U);
    }

    shared<BiConsumer> BiConsumer::andThen(const shared<BiConsumer> after) {
        return alloc<BiConsumer>([&](const shared<Object> &T, const shared<Object> &U) {
            accept(T, U);
            after->accept(T, U);
        });
    }
}
