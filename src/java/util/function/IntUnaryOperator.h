#pragma once

#include <Object.h>

namespace java::util::function {
    class IntUnaryOperator : public virtual Object {
        std::function<int(int)> inner;

    public:
        ~IntUnaryOperator() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        IntUnaryOperator(F&& func) : inner(std::forward<F>(func)) {}

        virtual shared<IntUnaryOperator> andThen(shared<IntUnaryOperator> after);

        virtual int applyAsInt(int i);

        virtual shared<IntUnaryOperator> compose(shared<IntUnaryOperator> before);

        static shared<IntUnaryOperator> identity();
    };
}
