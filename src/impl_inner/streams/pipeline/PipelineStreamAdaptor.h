#pragma once

#include <Object.h>
#include <java/util/stream/Stream.h>

namespace streams::pipeline {
    class AbstractPipeline;
}

namespace streams::pipeline {
    class PipelineStreamAdaptor : public virtual Object, public virtual java::util::stream::Stream {
        shared<AbstractPipeline> _pipeline;

    public:
        explicit PipelineStreamAdaptor(const shared<java::util::Spliterator>& splitr);

        explicit PipelineStreamAdaptor(const shared<AbstractPipeline>& pipeline);

        void close() override;

        bool isParallel() override;

        shared<java::util::Iterator> iterator() override;

        shared<Object> onClose(shared<Runnable> closeHandler) override;

        shared<Object> parallel() override;

        shared<Object> sequential() override;

        shared<java::util::Spliterator> spliterator() override;

        shared<Object> unordered() override;

        bool allMatch(shared<java::util::function::Predicate> predicate) override;

        bool anyMatch(shared<java::util::function::Predicate> predicate) override;

        shared<Object> collect(shared<java::util::function::Supplier> supplier,
            shared<java::util::function::BiConsumer> accumulator,
            shared<java::util::function::BiConsumer> combiner) override;

        shared<Object> collect(shared<java::util::stream::Collector> collector) override;

        long long count() override;

        shared<Stream> distinct() override;

        shared<Stream> filter(shared<java::util::function::Predicate> predicate) override;

        shared<java::util::Optional> findAny() override;

        shared<java::util::Optional> findFirst() override;

        shared<Stream> flatMap(shared<java::util::function::Function> mapper) override;

        shared<java::util::stream::DoubleStream>
        flatMapToDouble(shared<java::util::function::Function> mapper) override;

        shared<java::util::stream::IntStream> flatMapToInt(shared<java::util::function::Function> mapper) override;

        shared<java::util::stream::LongStream> flatMapToLong(shared<java::util::function::Function> mapper) override;

        void forEach(shared<java::util::function::Consumer> action) override;

        void forEachOrdered(shared<java::util::function::Consumer> action) override;

        shared<Stream> limit(long long maxSize) override;

        shared<Stream> map(shared<java::util::function::Function> mapper) override;

        shared<java::util::stream::DoubleStream> mapToDouble(shared<java::util::function::BiConsumer> mapper) override;

        shared<java::util::stream::IntStream> mapToInt(shared<java::util::function::ToIntFunction> mapper) override;

        shared<java::util::stream::LongStream> mapToLong(shared<java::util::function::ToLongFunction> mapper) override;

        shared<java::util::Optional> max(shared<java::util::Comparator> comparator) override;

        shared<java::util::Optional> min(shared<java::util::Comparator> comparator) override;

        bool noneMatch(shared<java::util::function::Predicate> predicate) override;

        shared<Stream> peek(shared<java::util::function::Consumer> action) override;

        shared<java::util::Optional> reduce(shared<java::util::function::BinaryOperator> accumulator) override;

        shared<Object> reduce(shared<Object> identity,
            shared<java::util::function::BinaryOperator> accumulator) override;

        shared<Object> reduce(shared<Object> identity, shared<java::util::function::BiFunction> accumulator,
            shared<java::util::function::BinaryOperator> combiner) override;

        shared<Stream> skip(long long n) override;

        shared<Stream> sorted() override;

        shared<Stream> sorted(shared<java::util::Comparator> comparator) override;

        shared<Array<>> toArray() override;

        shared<Array<>> toArray(shared<java::util::function::IntFunction> generator) override;
    };
}
