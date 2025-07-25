#pragma once

#include <Object.h>

namespace java::util::function {
    class DoubleToLongFunction : public virtual Object {
        std::function<long long(double)> inner;

    public:
        ~DoubleToLongFunction() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        DoubleToLongFunction(F&& func) : inner(std::forward<F>(func)) {}

        virtual long long applyAsLong(double value);
    };

}
