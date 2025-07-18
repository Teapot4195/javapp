#pragma once

#include <Object.h>

namespace java::util::function {
    class BiConsumer : public virtual Object {
        std::function<void(shared<Object>, shared<Object>)> inner;

    public:
        ~BiConsumer() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        BiConsumer(F&& func) : inner(std::forward<F>(func)) {}

        virtual void accept(shared<Object> T, shared<Object> U);

        virtual shared<BiConsumer> andThen(shared<BiConsumer> after);
    };
}
