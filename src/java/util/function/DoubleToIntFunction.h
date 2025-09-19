#pragma once

#include <Object.h>

namespace java::util::function {
    class DoubleToIntFunction : public virtual Object{
        std::function<int(double)> inner;

    public:
        ~DoubleToIntFunction() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        DoubleToIntFunction(F&& func) : inner(std::forward<F>(func)) {}

        virtual int applyasInt(double value);
    };

}
