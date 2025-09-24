#pragma once

#include <Object.h>

#include "BaseStream.h"

namespace java::util::function {
    class Supplier;
}

namespace java::util {
    class OptionalDouble;
}

namespace java::util::stream {
    class Stream;
    class LongStream;
}

namespace java::util::stream {
    class DoubleStream;
}

namespace java::util::function {
    class Predicate;
}

namespace java::util::stream {
    class IntStream : public virtual Object, public virtual BaseStream {
    public:
        ~IntStream() override = default;

        virtual bool allMatch(shared<function::Predicate> predicate) = 0;

        virtual bool anyMatch(shared<function::Predicate> predicate) = 0;

        virtual shared<DoubleStream> asDoubleStream() = 0;

        virtual shared<LongStream> asLongStream() = 0;

        virtual shared<OptionalDouble> average() = 0;

        virtual shared<Stream> boxed() = 0;

        // static Builder builder();

        // virtual shared<Object> collect(shared<function::Supplier> supplier, )
    };
}
