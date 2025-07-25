#pragma once

#include <Object.h>

namespace java::util::function {
    class DoubleBinaryOperator : public virtual Object {
        std::function<double(double, double)> inner;

    public:
        ~DoubleBinaryOperator() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        DoubleBinaryOperator(F&& func) : inner(std::forward<F>(func)) {}

        virtual double applyAsDouble(double left, double right);
    };
}
