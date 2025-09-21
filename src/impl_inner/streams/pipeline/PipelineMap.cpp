#include "PipelineMap.h"

#include <java/util/function/Function.h>

namespace streams::pipeline {
    PipelineMap::PipelineMap(const shared<AbstractPipeline> &before,
        const shared<java::util::function::Function> &mapper) : AbstractPipeline(before), mapper(mapper) {}

    shared<Object> PipelineMap::next() {
        return mapper->apply(_before->next());
    }
}
