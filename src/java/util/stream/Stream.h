#pragma once

#include <Array.h>

#include "BaseStream.h"
#include <Object.h>

#include "java/util/function/Consumer.h"

namespace java::util {
    class List;
    class Comparator;
    class Optional;

    namespace stream {
        class LongStream;
        class IntStream;
        class DoubleStream;
        class Collector;
    }

    namespace function {
        class IntFunction;
        class BiFunction;
        class BinaryOperator;
        class ToLongFunction;
        class ToIntFunction;
        class UnaryOperator;
        class Function;
        class Supplier;
        class BiConsumer;
        class Predicate;
    }
}

namespace java::util::stream {
    class Stream : public virtual Object, public virtual BaseStream {
    public:
        class Builder;

        ~Stream() override = default;

        virtual bool allMatch(shared<function::Predicate> predicate) = 0;

        virtual bool anyMatch(shared<function::Predicate> predicate) = 0;

        // static shared<Builder> builder();

        virtual shared<Object> collect(shared<function::Supplier> supplier, shared<function::BiConsumer> accumulator,
            shared<function::BiConsumer> combiner) = 0;

        virtual shared<Object> collect(shared<Collector> collector) = 0;

        // static shared<Stream> concat(shared<Stream> a, shared<Stream> b);

        virtual long long count() = 0;

        virtual shared<Stream> distinct() = 0;

        virtual shared<Stream> dropWhile(shared<function::Predicate> predicate);

        // static shared<Stream> empty();

        virtual shared<Stream> filter(shared<function::Predicate> predicate) = 0;

        virtual shared<Optional> findAny() = 0;

        virtual shared<Optional> findFirst() = 0;

        virtual shared<Stream> flatMap(shared<function::Function> mapper) = 0;

        virtual shared<DoubleStream> flatMapToDouble(shared<function::Function> mapper) = 0;

        virtual shared<IntStream> flatMapToInt(shared<function::Function> mapper) = 0;

        virtual shared<LongStream> flatMapToLong(shared<function::Function> mapper) = 0;

        virtual void forEach(shared<function::Consumer> action) = 0;

        virtual void forEachOrdered(shared<function::Consumer> action) = 0;

        // static shared<Stream> generate(shared<function::Supplier> s);

        // static shared<Stream> iterate(shared<Object> seed, shared<function::Predicate> hasNext, shared<function::UnaryOperator> next);

        // static shared<Stream> iterate(shared<Object> seed, shared<function::UnaryOperator> f);

        virtual shared<Stream> limit(long long maxSize) = 0;

        virtual shared<Stream> map(shared<function::Function> mapper) = 0;

        virtual shared<Stream> mapMulti(shared<function::BiConsumer> mapper);

        virtual shared<DoubleStream> mapMultiToDouble(shared<function::BiConsumer> mapper);

        virtual shared<IntStream> mapMultiToInt(shared<function::BiConsumer> mapper);

        virtual shared<LongStream> mapMultiToLong(shared<function::BiConsumer> mapper);

        virtual shared<DoubleStream> mapToDouble(shared<function::BiConsumer> mapper) = 0;

        virtual shared<IntStream> mapToInt(shared<function::ToIntFunction> mapper) = 0;

        virtual shared<LongStream> mapToLong(shared<function::ToLongFunction> mapper) = 0;

        virtual shared<Optional> max(shared<Comparator> comparator) = 0;

        virtual shared<Optional> min(shared<Comparator> comparator) = 0;

        virtual bool noneMatch(shared<function::Predicate> predicate) = 0;

        // static shared<Stream> of(std::vector<shared<Object>> ts);

        // template <typename... Args>
        // static shared<Stream> of(Args&&... values) {
        //     return of({values...});
        // }

        // static shared<Stream> ofNullable(shared<Object> t);

        virtual shared<Stream> peek(shared<function::Consumer> action) = 0;

        virtual shared<Optional> reduce(shared<function::BinaryOperator> accumulator) = 0;

        virtual shared<Object> reduce(shared<Object> identity, shared<function::BinaryOperator> accumulator) = 0;

        virtual shared<Object> reduce(shared<Object> identity, shared<function::BiFunction> accumulator,
            shared<function::BinaryOperator> combiner) = 0;

        virtual shared<Stream> skip(long long n) = 0;

        virtual shared<Stream> sorted() = 0;

        virtual shared<Stream> sorted(shared<Comparator> comparator) = 0;

        virtual shared<Stream> takeWhile(shared<function::Predicate> predicate);

        virtual shared<Array<>> toArray() = 0;

        virtual shared<Array<>> toArray(shared<function::IntFunction> generator) = 0;

        virtual shared<List> toList();

    };

    class Stream::Builder : public virtual Object, public virtual function::Consumer {
    public:
        ~Builder() override = default;

        void accept(shared<Object> T) override = 0;

        virtual shared<Builder> add(shared<Object> T);

        virtual shared<Stream> build() = 0;
    };
}
