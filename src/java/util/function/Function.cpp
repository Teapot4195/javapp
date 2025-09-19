#include "Function.h"

namespace java::util::function {
    shared<Function> Function::andThen(const shared<Function> after) {
        return alloc<Function>(lambda(const shared<Object> &t) {
            return after->apply(self->apply(t));
        });
    }

    shared<Object> Function::apply(const shared<Object> t) {
        return inner(t);
    }

    shared<Function> Function::compose(const shared<Function> before) {
        return alloc<Function>(lambda(const shared<Object> &t) {
            return self->apply(before->apply(t));
        });
    }

    shared<Function> Function::identity() {
        return alloc<Function>([](const shared<Object> &t) {
            return t;
        });
    }
}
