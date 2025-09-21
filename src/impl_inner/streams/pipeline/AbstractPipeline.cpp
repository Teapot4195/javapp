#include "AbstractPipeline.h"

#include <streams/pipeline/PipelineFlatMap.h>
#include <streams/pipeline/PipelineLimit.h>
#include <streams/pipeline/PipelinePeek.h>
#include <streams/pipeline/PipelineSkip.h>
#include <streams/pipeline/PipelineDistinctEntries.h>
#include <streams/pipeline/PipelineFilterEntries.h>
#include <streams/pipeline/PipelineSpliteratorSource.h>

namespace streams::pipeline {
    shared<Object> AbstractPipeline::next() {
        return _before->next();
    }

    bool AbstractPipeline::hasNext() {
        return _before->hasNext();
    }

    shared<AbstractPipeline> AbstractPipeline::fromSpliterator(shared<java::util::Spliterator> spliterator) {
        return alloc<PipelineSpliteratorSource>(spliterator);
    }

    shared<AbstractPipeline> AbstractPipeline::transformDistinct(const shared<AbstractPipeline> &pipeline) {
        return alloc<PipelineDistinctEntries>(pipeline);
    }

    shared<AbstractPipeline> AbstractPipeline::transformFilter(const shared<AbstractPipeline> &pipeline,
        const shared<java::util::function::Predicate> &predicate) {
        return alloc<PipelineFilterEntries>(pipeline, predicate);
    }

    shared<AbstractPipeline> AbstractPipeline::transformFlatMap(const shared<AbstractPipeline> &pipeline,
        const shared<java::util::function::Function> &mapper) {
        return alloc<PipelineFlatMap>(pipeline, mapper);
    }

    shared<AbstractPipeline> AbstractPipeline::transformLimit(const shared<AbstractPipeline> &pipeline,
        long long limit) {
        return alloc<PipelineLimit>(pipeline, limit);
    }

    shared<AbstractPipeline> AbstractPipeline::transformMap(const shared<AbstractPipeline> &pipeline,
        const shared<java::util::function::Function> &mapper) {
        return alloc<PipelineFlatMap>(pipeline, mapper);
    }

    shared<AbstractPipeline> AbstractPipeline::transformPeek(const shared<AbstractPipeline> &pipeline,
        const shared<java::util::function::Consumer> &mapper) {
        return alloc<PipelinePeek>(pipeline, mapper);
    }

    shared<AbstractPipeline> AbstractPipeline::
    transformSkip(const shared<AbstractPipeline> &pipeline, long long count) {
        return alloc<PipelineSkip>(pipeline, count);
    }

    shared<AbstractPipeline> AbstractPipeline::split() {
        return _before->split();
    }

    void AbstractPipeline::close() {
        _before->close();
    }
}
