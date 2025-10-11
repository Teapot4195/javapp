#include "IntPipelineMapToObj.h"

#include <java/util/function/IntFunction.h>
#include <streams/pipeline/AbstractIntPipeline.h>

namespace streams::pipeline {
    IntPipelineMapToObj::IntPipelineMapToObj(const shared<AbstractIntPipeline> &source_pipeline,
        const shared<java::util::function::IntFunction> &conversion_function): source_pipeline_(source_pipeline),
                                                                               conversion_function_(conversion_function) {}

    bool IntPipelineMapToObj::hasNext() {
        return source_pipeline_->hasNext();
    }

    shared<Object> IntPipelineMapToObj::next() {
        return conversion_function_->apply(source_pipeline_->next());
    }

    void IntPipelineMapToObj::close() {
        source_pipeline_->close();
    }
}
