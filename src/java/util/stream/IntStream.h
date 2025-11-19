#pragma once

#include <Array.h>
#include <Object.h>

#include "BaseStream.h"

namespace java::util {
    class IntSummaryStatistics;
}

namespace java::util::function {
    class IntBinaryOperator;
}

namespace java::util {
    class OptionalInt;
    class OptionalDouble;

    namespace function {
        class IntToLongFunction;
        class IntToDoubleFunction;
        class BiConsumer;
        class IntConsumer;
        class IntFunction;
        class IntPredicate;
        class IntSupplier;
        class IntUnaryOperator;
        class ObjIntConsumer;
        class Predicate;
        class Supplier;
    }

    namespace stream {
        class DoubleStream;
        class LongStream;
        class Stream;
    }
}

namespace java::util::stream {
    class IntStream : public virtual Object, public virtual BaseStream {
    public:
        class IntMapMultiConsumer : public virtual Object {
            std::function<void(int, shared<function::IntConsumer>)> inner;

        public:
            ~IntMapMultiConsumer() override = default;

            /**
                 * Implicit conversion function
                 * @tparam F function type
                 * @param func the inner function
                 */
            template <typename F>
            IntMapMultiConsumer(F&& func) : inner(std::forward<F>(func)) {}

            virtual void accept(int value, shared<function::IntConsumer> ic);
        };

        ~IntStream() override = default;

        virtual bool allMatch(shared<function::IntPredicate> predicate) = 0;

        virtual bool anyMatch(shared<function::IntPredicate> predicate) = 0;

        virtual shared<DoubleStream> asDoubleStream() = 0;

        virtual shared<LongStream> asLongStream() = 0;

        virtual shared<OptionalDouble> average() = 0;

        virtual shared<Stream> boxed() = 0;

        // static Builder builder();

        virtual shared<Object> collect(shared<function::Supplier> supplier, shared<function::ObjIntConsumer> accumulator, shared<function::BiConsumer> combiner) = 0;

        static shared<IntStream> concat(shared<IntStream> a, shared<IntStream> b);

        virtual long long count() = 0;

        virtual shared<IntStream> distinct() = 0;

        virtual shared<IntStream> dropWhile(shared<function::IntPredicate> predicate);

        static shared<IntStream> empty();

        virtual shared<IntStream> filter(shared<function::IntPredicate> predicate) = 0;

        virtual shared<OptionalInt> findAny() = 0;

        virtual shared<OptionalInt> findFirst() = 0;

        virtual shared<IntStream> flatMap(shared<function::IntFunction> mapper) = 0;

        virtual void forEach(shared<function::IntConsumer> action) = 0;

        virtual void forEachOrdered(shared<function::IntConsumer> action) = 0;

        static shared<IntStream> generate(shared<function::IntSupplier> s);

        static shared<IntStream> iterate(int seed, shared<function::IntPredicate> hasNext, shared<function::IntUnaryOperator> next);

        static shared<IntStream> iterate(int seed, shared<function::IntUnaryOperator> f);

        virtual shared<IntStream> limit(long long maxSize) = 0;

        virtual shared<IntStream> map(shared<function::IntUnaryOperator> mapper) = 0;

        virtual shared<IntStream> mapMulti(shared<IntMapMultiConsumer> mapper);

        virtual shared<DoubleStream> mapToDouble(shared<function::IntToDoubleFunction> mapper) = 0;

        virtual shared<LongStream> mapToLong(shared<function::IntToLongFunction> mapper) = 0;

        virtual shared<Stream> mapToObj(shared<function::IntFunction> mapper) = 0;

        virtual shared<OptionalInt> max() = 0;

        virtual shared<OptionalInt> min() = 0;

        virtual bool noneMatch(shared<function::IntPredicate> predicate) = 0;

        static shared<IntStream> of(std::initializer_list<int> values);

        template <typename... Args>
        static shared<IntStream> of(Args&&... values) {
            return of({std::forward<Args>(values)...});
        }

        virtual shared<IntStream> peek(shared<function::IntConsumer> action) = 0;

        static shared<IntStream> range(int startInclusive, int endInclusive);

        static shared<IntStream> rangeClosed(int startInclusive, int endInclusive);

        virtual int reduce(int identity, shared<function::IntBinaryOperator> op) = 0;

        virtual shared<OptionalInt> reduce(shared<function::IntBinaryOperator> op) = 0;

        virtual shared<IntStream> skip(long long n) = 0;

        virtual int sum() = 0;

        virtual shared<IntSummaryStatistics> summaryStatistics() = 0;

        virtual shared<IntStream> takeWhile(shared<function::IntPredicate> predicate) = 0;

        virtual shared<Array<int>> toArray() = 0;
    };
}
