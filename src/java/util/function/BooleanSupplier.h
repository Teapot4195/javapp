#pragma once

#include <Object.h>

namespace java::util::function {
    class BooleanSupplier : public virtual Object {
        std::function<bool()> inner;

    public:
        ~BooleanSupplier() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        BooleanSupplier(F&& func) : inner(std::forward<F>(func)) {}

        virtual bool getAsBoolean();
    };

}
