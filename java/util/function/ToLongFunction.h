#pragma once

#include <Object.h>

namespace java::util::function {
    class ToLongFunction : public virtual Object {
        std::function<long long(shared<Object>)> inner;

    public:
        ~ToLongFunction() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        ToLongFunction(F&& func) : inner(std::forward<F>(func)) {}

        virtual long long applyAsLong(shared<Object> value);
    };

}
