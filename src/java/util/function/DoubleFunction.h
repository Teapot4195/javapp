#pragma once

#include <Object.h>

namespace java::util::function {
    class DoubleFunction : public virtual Object {
        std::function<shared<Object>(double)> inner;

    public:
        ~DoubleFunction() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        DoubleFunction(F&& func) : inner(std::forward<F>(func)) {}

        virtual shared<Object> apply(double value);
    };

}
