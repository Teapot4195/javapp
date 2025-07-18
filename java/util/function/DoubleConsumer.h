#pragma once

#include <Object.h>

namespace java::util::function {
    class DoubleConsumer : public virtual Object {
        std::function<void(double)> inner;

    public:
        ~DoubleConsumer() override = default;

        /**
         * Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        DoubleConsumer(F&& func) : inner(std::forward<F>(func)) {}

        /**
         * @brief Default constructor, allows the inner function to be ignored if this it's overriden?
         */
        DoubleConsumer() = default;

        virtual void accept(double value);

        virtual shared<DoubleConsumer> andThen(shared<DoubleConsumer> after);
    };

}
