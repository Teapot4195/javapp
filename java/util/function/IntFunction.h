#pragma once

#include <Object.h>

namespace java::util::function {
    class IntFunction : public virtual Object {
        std::function<shared<Object>(int)> inner;

    public:
        ~IntFunction() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        IntFunction(F&& func) : inner(std::forward<F>(func)) {}

        virtual shared<Object> apply(int value);
    };
}
