#include "BiFunction.h"

#include "Function.h"

#include <utility>

namespace java::util::function {
    shared<BiFunction> BiFunction::andThen(shared<Function> after) {
        return alloc<BiFunction>([&](const shared<Object> &T, const shared<Object> &U) -> shared<Object> {
            return after->apply(apply(T, U));
        });
    }

    shared<Object> BiFunction::apply(shared<Object> T, shared<Object> U) {
        return inner(std::move(T), std::move(U));
    }
}
