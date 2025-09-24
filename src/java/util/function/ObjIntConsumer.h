#pragma once

#include <Object.h>

namespace java::util::function {
    class ObjIntConsumer : public virtual Object {
        std::function<void(shared<Object>, int)> inner;

    public:
        ~ObjIntConsumer() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        ObjIntConsumer(F&& func) : inner(std::forward<F>(func)) {}

        virtual void accept(shared<Object> t, int value);
    };
}
