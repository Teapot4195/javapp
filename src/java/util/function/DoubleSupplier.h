#pragma once

#include <Object.h>

namespace java::util::function {
    class DoubleSupplier : public virtual Object {
        std::function<double()> inner;

    public:
        ~DoubleSupplier() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        DoubleSupplier(F&& func) : inner(std::forward<F>(func)) {}

        virtual double getAsDouble();
    };
}
