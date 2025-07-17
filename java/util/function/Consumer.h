#pragma once

#include <Object.h>

namespace java::util::function {
    class Consumer : public virtual Object {
        std::function<void(shared<Object>)> inner;
    public:
        ~Consumer() override = default;

        /**
         * Implicit conversion function
         * @tparam F function type
         * @param func
         */
        template<typename F>
        Consumer(F&& func) : inner(func) {}

        virtual void accept(shared<Object> T);

        virtual shared<Consumer> andThen(shared<Consumer> after);
    };
}
