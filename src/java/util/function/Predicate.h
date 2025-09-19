#pragma once

#include <Object.h>

namespace java::util::function {
    class Predicate : public virtual Object {
        std::function<bool(shared<Object>)> inner;

    public:
        ~Predicate() override = default;

        /**
         * Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        Predicate(F&& func) : inner(func) {}

        virtual shared<Predicate> And(shared<Predicate> other);

        static shared<Predicate> isEqual(const shared<Object> &targetRef);

        virtual shared<Predicate> negate();

        static shared<Predicate> Not(const shared<Predicate> &target);

        virtual shared<Predicate> Or(shared<Predicate> other);

        virtual bool test(shared<Object> value);
    };
}
