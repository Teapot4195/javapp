#pragma once

#include <streams/pipeline/AbstractPipeline.h>

namespace java::util::function {
    class Function;
}

namespace streams::pipeline {
    class PipelineFlatMap : public virtual Object, public virtual AbstractPipeline {
        shared<java::util::function::Function> mapper;
        shared<java::util::Spliterator> splitr;
        std::optional<shared<Object>> _next;
        shared<java::util::function::Consumer> _cons;

    public:
        PipelineFlatMap(const shared<AbstractPipeline> &before, const shared<java::util::function::Function> &mapper);

        bool hasNext() override;

        shared<Object> next() override;

        bool fillNextBuffer();
    };
}
