#include "Function.h"

namespace java::util::function {
    shared<Function> Function::andThen(const shared<Function> after) {
        return alloc<Function>([&](const shared<Object> &t) {
            return after->apply(apply(t));
        });
    }

    shared<Object> Function::apply(const shared<Object> t) {
        return inner(t);
    }

    shared<Function> Function::compose(const shared<Function> before) {
        return alloc<Function>([&](const shared<Object> &t) {
            return apply(before->apply(t));
        });
    }

    shared<Function> Function::identity() {
        return alloc<Function>([](const shared<Object> &t) {
            return t;
        });
    }
}
