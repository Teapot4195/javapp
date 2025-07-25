#pragma once

#include <Object.h>

namespace java::util::function {
    class DoubleUnaryOperator : public virtual Object {
        std::function<double(double)> inner;

    public:
        ~DoubleUnaryOperator() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        DoubleUnaryOperator(F&& func) : inner(std::forward<F>(func)) {}

        virtual shared<DoubleUnaryOperator> andThen(shared<DoubleUnaryOperator> after);

        virtual double applyAsDouble(double value);

        virtual shared<DoubleUnaryOperator> compose(shared<DoubleUnaryOperator> before);

        static shared<DoubleUnaryOperator> identity();
    };

}
