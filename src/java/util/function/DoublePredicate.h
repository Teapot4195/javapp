#pragma once

#include <Object.h>

namespace java::util::function {
    class DoublePredicate : public virtual Object {
        std::function<bool(double)> inner;

    public:
        ~DoublePredicate() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        DoublePredicate(F&& func) : inner(std::forward<F>(func)) {}

        virtual shared<DoublePredicate> And(shared<DoublePredicate> other);

        virtual shared<DoublePredicate> negate();

        virtual shared<DoublePredicate> Or(shared<DoublePredicate> other);

        virtual bool test(double value);
    };
}
