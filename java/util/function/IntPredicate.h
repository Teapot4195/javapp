#pragma once

#include <Object.h>

namespace java::util::function {
    class IntPredicate : public virtual Object {
        std::function<bool(int)> inner;

    public:
        ~IntPredicate() override = default;

        /**
         * Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        IntPredicate(F&& func) : inner(func) {}

        virtual shared<IntPredicate> And(shared<IntPredicate> other);

        virtual shared<IntPredicate> negate();

        virtual shared<IntPredicate> Or(shared<IntPredicate> other);

        virtual bool test(int value);
    };
}
