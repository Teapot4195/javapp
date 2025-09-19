#pragma once

#include <AutoCloseable.h>
#include <Object.h>

namespace java::util {
    class Spliterator;
}

class Runnable;

namespace java::util {
    class Iterator;
}

namespace java::util::stream {
    class BaseStream : public virtual Object, public virtual AutoCloseable {
    public:
        ~BaseStream() override = default;

        void close() override = 0;

        virtual bool isParallel() = 0;

        virtual shared<Iterator> iterator() = 0;

        virtual shared<Object> onClose(shared<Runnable> closeHandler) = 0;

        virtual shared<Object> parallel() = 0;

        virtual shared<Object> sequential() = 0;

        virtual shared<Spliterator> spliterator() = 0;

        virtual shared<Object> unordered() = 0;
    };
}
