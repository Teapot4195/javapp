#pragma once

#include <Object.h>

namespace java::util {
    namespace function {
        class Consumer;
    }

    class Iterator : public virtual Object {
    public:
        ~Iterator() override = default;

        virtual void forEachRemaining(shared<function::Consumer> action);

        virtual bool hasNext() = 0;

        virtual shared<Object> next() = 0;

        virtual void remove();
    };
}
