#pragma once

#include "Object.h"

namespace java::util::function {
    class IntConsumer : public virtual Object {
        std::function<void(int)> inner;

    public:
        ~IntConsumer() override = default;

        /**
             * Implicit conversion function
             * @tparam F function type
             * @param func the inner function
             */
        template <typename F>
        IntConsumer(F&& func) : inner(std::forward<F>(func)) {}

        /**
         * @brief Default constructor, allows the inner function to be ignored if this it's overriden
         */
        IntConsumer() = default;

        virtual void accept(int value);

        virtual shared<IntConsumer> andThen(shared<IntConsumer> after);
    };
}
