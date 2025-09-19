#pragma once

#include <Object.h>

namespace java::util::function {
    class LongConsumer : public virtual Object {
        std::function<void(long long)> inner;

    public:
        ~LongConsumer() override = default;

        /**
             * Implicit conversion function
             * @tparam F function type
             * @param func the inner function
             */
        template <typename F>
        LongConsumer(F&& func) : inner(std::forward<F>(func)) {}

        /**
         * @brief Default constructor, allows the inner function to be ignored if this it's overriden
         */
        LongConsumer() = default;

        virtual void accept(long long value);

        virtual shared<LongConsumer> andThen(shared<LongConsumer> after);
    };
}
