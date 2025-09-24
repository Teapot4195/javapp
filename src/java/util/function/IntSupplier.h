#pragma once

#include <Object.h>

namespace java::util::function {
    class IntSupplier : public virtual Object {
        std::function<int()> inner;

    public:
        ~IntSupplier() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        IntSupplier(F&& func) : inner(std::forward<F>(func)) {}

        virtual int getAsInt();
    };
}
