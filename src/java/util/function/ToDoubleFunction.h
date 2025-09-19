#pragma once

#include <Object.h>

namespace java::util::function {
    class ToDoubleFunction : public virtual Object {
        std::function<double(shared<Object>)> inner;

    public:
        ~ToDoubleFunction() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        ToDoubleFunction(F&& func) : inner(std::forward<F>(func)) {}

        virtual double applyAsDouble(shared<Object> value);
    };

}
