#include "PipelineStreamAdaptor.h"

#include <ranges>
#include <java/util/ArrayList.h>
#include <java/util/Comparator.h>
#include <java/util/function/BiConsumer.h>
#include <java/util/function/Function.h>
#include <java/util/function/Predicate.h>
#include <java/util/function/Supplier.h>
#include <java/util/Optional.h>
#include <java/util/function/BinaryOperator.h>
#include <java/util/stream/Collector.h>
#include <streams/pipeline/AbstractPipeline.h>
#include <streams/pipeline/PipelineArrayListSource.h>

namespace streams::pipeline {
    using namespace java::util;

    PipelineStreamAdaptor::PipelineStreamAdaptor(const shared<Spliterator> &splitr) {
        _pipeline = AbstractPipeline::fromSpliterator(splitr);
    }

    PipelineStreamAdaptor::PipelineStreamAdaptor(const shared<AbstractPipeline> &pipeline) {
        _pipeline = pipeline;
    }

    bool PipelineStreamAdaptor::allMatch(const shared<function::Predicate> predicate) {
        while (_pipeline->hasNext()) {
            if (!predicate->test(_pipeline->next())) {
                return false;
            }
        }

        return true;
    }

    bool PipelineStreamAdaptor::anyMatch(const shared<function::Predicate> predicate) {
        while (_pipeline->hasNext()) {
            if (predicate->test(_pipeline->next())) {
                return true;
            }
        }

        return false;
    }

    shared<Object> PipelineStreamAdaptor::collect(const shared<function::Supplier> supplier,
        const shared<function::BiConsumer> accumulator, shared<function::BiConsumer> combiner) {
        shared<Object> result = supplier->get();

        while (_pipeline->hasNext()) {
            accumulator->accept(result, _pipeline->next());
        }

        return result;
    }

    shared<Object> PipelineStreamAdaptor::collect(const shared<stream::Collector> collector) {
        const shared<Object> result = collector->supplier()->get();

        while (_pipeline->hasNext()) {
            collector->accumulator()->accept(result, _pipeline->next());
        }

        return collector->finisher()->apply(result);
    }

    long long PipelineStreamAdaptor::count() {
        long long _count = 0;

        while (_pipeline->hasNext()) {
            _count++;
            _pipeline->next();
        }

        return _count;
    }

    shared<stream::Stream> PipelineStreamAdaptor::distinct() {
        return alloc<PipelineStreamAdaptor>(AbstractPipeline::transformDistinct(_pipeline));
    }

    shared<stream::Stream> PipelineStreamAdaptor::
    filter(shared<function::Predicate> predicate) {
        return alloc<PipelineStreamAdaptor>(AbstractPipeline::transformFilter(_pipeline, predicate));
    }

    shared<Optional> PipelineStreamAdaptor::findAny() {
        // TODO: could be parallelized
        if (_pipeline->hasNext())
            return alloc<Optional>(_pipeline->next());
        return Optional::empty();
    }

    shared<Optional> PipelineStreamAdaptor::findFirst() {
        if (_pipeline->hasNext())
            return alloc<Optional>(_pipeline->next());
        return Optional::empty();
    }

    shared<stream::Stream> PipelineStreamAdaptor::flatMap(const shared<function::Function> mapper) {
        return alloc<PipelineStreamAdaptor>(AbstractPipeline::transformFlatMap(_pipeline, mapper));
    }

    void PipelineStreamAdaptor::forEach(const shared<function::Consumer> action) {
        // TODO: could be parallelized
        while (_pipeline->hasNext())
            action->accept(_pipeline->next());
    }

    void PipelineStreamAdaptor::forEachOrdered(const shared<function::Consumer> action) {
        while (_pipeline->hasNext())
            action->accept(_pipeline->next());
    }

    shared<stream::Stream> PipelineStreamAdaptor::limit(const long long maxSize) {
        return alloc<PipelineStreamAdaptor>(AbstractPipeline::transformLimit(_pipeline, maxSize));
    }

    shared<stream::Stream> PipelineStreamAdaptor::map(const shared<function::Function> mapper) {
        return alloc<PipelineStreamAdaptor>(AbstractPipeline::transformMap(_pipeline, mapper));
    }

    shared<Optional> PipelineStreamAdaptor::max(shared<Comparator> comparator) {
        if (_pipeline->hasNext())
            return Optional::empty();
        shared<Object> result = _pipeline->next();

        while (_pipeline->hasNext()) {
            if (const shared<Object> current = _pipeline->next(); comparator->compare(result, current) < 0)
                result = current;
        }

        return Optional::empty();
    }

    shared<Optional> PipelineStreamAdaptor::min(shared<Comparator> comparator) {
        if (_pipeline->hasNext())
            return Optional::empty();
        shared<Object> result = _pipeline->next();

        while (_pipeline->hasNext()) {
            if (const shared<Object> current = _pipeline->next(); comparator->compare(result, current) > 0)
                result = current;
        }

        return Optional::empty();
    }

    bool PipelineStreamAdaptor::noneMatch(shared<function::Predicate> predicate) {
        while (_pipeline->hasNext())
            if (predicate->test(_pipeline->next()))
                return false;

        return true;
    }

    shared<stream::Stream> PipelineStreamAdaptor::peek(shared<function::Consumer> action) {
        return alloc<PipelineStreamAdaptor>(AbstractPipeline::transformPeek(_pipeline, action));
    }

    shared<Optional> PipelineStreamAdaptor::reduce(shared<function::BinaryOperator> accumulator) {
        std::optional<shared<Object>> result;

        while (_pipeline->hasNext()) {
            if (result.has_value())
                result = accumulator->apply(result.value(), _pipeline->next());
            else {
                result = _pipeline->next();
            }
        }

        return result.has_value() ? alloc<Optional>(result.value()) : Optional::empty();
    }

    shared<Object> PipelineStreamAdaptor::reduce(shared<Object> identity, shared<function::BinaryOperator> accumulator) {
        shared<Object> result = identity;

        while (_pipeline->hasNext()) {
            result = accumulator->apply(result, _pipeline->next());
        }

        return result;
    }

    shared<Object> PipelineStreamAdaptor::reduce(shared<Object> identity, shared<function::BiFunction> accumulator,
        shared<function::BinaryOperator> combiner) {
        shared<Object> result = identity;

        while (_pipeline->hasNext())
            result = accumulator->apply(result, _pipeline->next());

        return result;
    }

    shared<stream::Stream> PipelineStreamAdaptor::skip(long long n) {
        return alloc<PipelineStreamAdaptor>(AbstractPipeline::transformSkip(_pipeline, n));
    }

    shared<stream::Stream> PipelineStreamAdaptor::sorted() {
        // drain the entire stream
        shared<ArrayList> list = alloc<ArrayList>();

        while (_pipeline->hasNext())
            list->add(_pipeline->next());

        list->sort();

        return alloc<PipelineStreamAdaptor>(alloc<PipelineArrayListSource>(list));
    }

    shared<stream::Stream> PipelineStreamAdaptor::sorted(shared<java::util::Comparator> comparator) {
        // drain the entire stream
        shared<ArrayList> list = alloc<ArrayList>();

        while (_pipeline->hasNext())
            list->add(_pipeline->next());

        list->sort(comparator);

        return alloc<PipelineStreamAdaptor>(alloc<PipelineArrayListSource>(list));
    }

    shared<Array<>> PipelineStreamAdaptor::toArray() {
        shared<ArrayList> list = alloc<ArrayList>();

        while (_pipeline->hasNext())
            list->add(_pipeline->next());

        return list->toArray();
    }

    shared<Array<>> PipelineStreamAdaptor::toArray(shared<function::IntFunction> generator) {
        shared<ArrayList> list = alloc<ArrayList>();

        while (_pipeline->hasNext())
            list->add(_pipeline->next());

        return list->toArray(generator);
    }
}
