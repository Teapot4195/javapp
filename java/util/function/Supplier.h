#pragma once

#include <Object.h>

namespace java::util::function {
    class Supplier : public virtual Object {
        std::function<shared<Object>()> inner;

    public:
        ~Supplier() override = default;

        /**
         * Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        Supplier(F&& func) : inner(std::forward<F>(func)) {}

        virtual shared<Object> get();
    };

}
