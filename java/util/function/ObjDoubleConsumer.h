#pragma once

#include <Object.h>

namespace java::util::function {
    class ObjDoubleConsumer : public virtual Object {
        std::function<void(shared<Object>, double)> inner;

    public:
        ~ObjDoubleConsumer() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        ObjDoubleConsumer(F&& func) : inner(std::forward<F>(func)) {}

        virtual void accept(shared<Object> t, double value);
    };
}
