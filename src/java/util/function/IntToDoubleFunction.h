#pragma once

#include <Object.h>

namespace java::util::function {
    class IntToDoubleFunction : public virtual Object {
        std::function<double(int)> inner;

    public:
        ~IntToDoubleFunction() override = default;

        /**
             * Implicit conversion function
             * @tparam F function type
             * @param func the inner function
             */
        template <typename F>
        IntToDoubleFunction(F&& func) : inner(std::forward<F>(func)) {}

        virtual double applyAsDouble(int value);
    };
}
