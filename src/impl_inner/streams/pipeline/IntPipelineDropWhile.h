#pragma once

#include <Object.h>
#include <streams/pipeline/AbstractIntPipeline.h>

namespace java::util::function {
    class IntPredicate;
}

namespace streams::pipeline {
    class IntPipelineDropWhile : public virtual Object, public virtual AbstractIntPipeline {
        shared<java::util::function::IntPredicate> predicate;
        bool dropped = false;
        std::optional<int> _next;

    public:
        IntPipelineDropWhile(shared<AbstractIntPipeline> pipeline, shared<java::util::function::IntPredicate> predicate);

        bool hasNext() override;

        int next() override;
    };
}
