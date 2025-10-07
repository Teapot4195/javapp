#pragma once

#include <Object.h>

namespace java::util::function {
    class IntToLongFunction : public virtual Object {
        std::function<long long(int)> inner;

    public:
        ~IntToLongFunction() override = default;

        /**
             * Implicit conversion function
             * @tparam F function type
             * @param func the inner function
             */
        template <typename F>
        IntToLongFunction(F&& func) : inner(std::forward<F>(func)) {}

        virtual long long applyAsDouble(int value);
    };
}
