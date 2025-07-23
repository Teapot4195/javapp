#pragma once

#include <Object.h>

namespace java::util::function {
    class ToIntFunction : public virtual Object {
        std::function<int(shared<Object>)> inner;

    public:
        ~ToIntFunction() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        ToIntFunction(F&& func) : inner(std::forward<F>(func)) {}

        virtual int applyAsInt(shared<Object> value);
    };

}
