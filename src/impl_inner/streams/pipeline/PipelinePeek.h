#pragma once

#include <impl_inner/streams/pipeline/AbstractPipeline.h>

namespace streams::pipeline {
    class PipelinePeek : public virtual Object, public virtual AbstractPipeline {
        shared<java::util::function::Consumer> _cons;

    public:
        PipelinePeek(const shared<AbstractPipeline> &before, const shared<java::util::function::Consumer> &consumer);

        shared<Object> next() override;
    };
}
