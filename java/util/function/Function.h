#pragma once

#include <Object.h>

namespace java::util::function {
    class Function : public virtual Object {
        std::function<shared<Object>(shared<Object>)> inner;

    public:
        ~Function()() override = default;

        /**
         * Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        Function(F&& func) : inner(func) {}

        virtual shared<Function> andThen(shared<Function> after);

        virtual shared<Object> apply(shared<Object> t);

        virtual shared<Function> compose(shared<Function> before);

        static shared<Function> identity();
    };

}
