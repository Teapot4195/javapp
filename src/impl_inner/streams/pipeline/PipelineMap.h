#pragma once

#include <streams/pipeline/AbstractPipeline.h>

namespace java::util::function {
    class Function;
}

namespace streams::pipeline {
    class PipelineMap : public virtual Object, public virtual AbstractPipeline {
        shared<java::util::function::Function> mapper;

    public:
        PipelineMap(const shared<AbstractPipeline> &before, const shared<java::util::function::Function> &mapper);

        shared<Object> next() override;
    };
}

