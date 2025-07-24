#pragma once

#include <Object.h>

namespace java::util::function {
    class BiPredicate : public virtual Object {
        std::function<bool(shared<Object>, shared<Object>)> inner;

    public:
        ~BiPredicate() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        BiPredicate(F&& func) : inner(std::forward<F>(func)) {}

        virtual shared<BiPredicate> And(shared<BiPredicate> other);

        virtual shared<BiPredicate> negate();

        virtual shared<BiPredicate> Or(shared<BiPredicate> other);

        virtual bool test(shared<Object> t, shared<Object> u);
    };
}
