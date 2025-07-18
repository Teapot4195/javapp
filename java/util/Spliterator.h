#pragma once

#include <Object.h>

#include "Spliterator.h"

namespace java::util {
    class Comparator;
}

namespace java::util::function {
    class Consumer;
}

namespace java::util {
    class Spliterator : public virtual Object {
    public:
        static constexpr int DISTINCT = 1;
        static constexpr int SORTED = 1 << 2;
        static constexpr int ORDERED = 1 << 4;
        static constexpr int SIZED = 1 << 6;
        static constexpr int NONNULL = 1 << 8;
        static constexpr int IMMUTABLE = 1 << 10;
        static constexpr int CONCURRENT = 1 << 12;
        static constexpr int SUBSIZED = 1 << 14;

        ~Spliterator() override = default;

        virtual int characteristics() = 0;

        virtual long long estimateSize() = 0;

        virtual void forEachRemaining(shared<function::Consumer> action);

        virtual shared<Comparator> getComparator();

        virtual long long getExactSizeIfKnown();

        virtual bool hasCharacteristics(int characteristics);

        virtual bool tryAdvance(shared<function::Consumer> action) = 0;

        virtual shared<Spliterator> trySplit() = 0;

        class OfDouble;
        class OfInt;
        class OfLong;
        class OfPrimitive;
    };

#warning PARTIAL NONCOMPLIANCE!!!

    class Spliterator::OfPrimitive : public virtual Spliterator {
    public:
        ~OfPrimitive() override = default;
    };

    class Spliterator::OfLong : public virtual OfPrimitive {
    public:
        ~OfLong() override = default;
    };

    class Spliterator::OfInt : public virtual OfPrimitive {
    public:
        ~OfInt() override = default;
    };

    class Spliterator::OfDouble : public virtual OfPrimitive {
    public:
        ~OfDouble() override = default;
    };
}
