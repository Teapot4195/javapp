#include "IntPipelineBoxer.h"

#include <Integer.h>
#include <java/util/function/Consumer.h>
#include <streams/pipeline/AbstractIntPipeline.h>

namespace streams::pipeline {
    IntPipelineBoxer::IntPipelineBoxer(const shared<AbstractIntPipeline> &source) : source_(source) {}

    void IntPipelineBoxer::forEachRemaining(shared<java::util::function::Consumer> action) {
        while (source_->hasNext())
            action->accept(Integer::valueOf(source_->next()));
    }

    shared<Object> IntPipelineBoxer::next() {
        return Integer::valueOf(source_->next());
    }

    bool IntPipelineBoxer::hasNext() {
        return source_->hasNext();
    }

    void IntPipelineBoxer::close() {
        return source_->close();
    }

    IntPipelineBoxer::~IntPipelineBoxer() = default;
}
