#pragma once

#include <Object.h>

namespace java::util::function {
    class IntBinaryOperator : public virtual Object {
        std::function<int(int, int)> inner;

    public:
        ~IntBinaryOperator() override = default;

        /**
             * Implicit conversion function
             * @tparam F function type
             * @param func the inner function
             */
        template <typename F>
        IntBinaryOperator(F&& func) : inner(std::forward<F>(func)) {}

        virtual int applyAsInt(int left, int right);
    };
}
