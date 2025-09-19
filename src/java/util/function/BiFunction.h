#pragma once

#include <Object.h>

namespace java::util::function {
    class Function;

    class BiFunction : public virtual Object {
        std::function<shared<Object>(shared<Object>, shared<Object>)> inner;

    public:
        ~BiFunction() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        BiFunction(F&& func) : inner(std::forward<F>(func)) {}

        virtual shared<BiFunction> andThen(shared<Function> after);

        virtual shared<Object> apply(shared<Object> T, shared<Object> U);
    };

}
