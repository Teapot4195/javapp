#pragma once

#include <Object.h>
#include <streams/pipeline/AbstractIntPipeline.h>

namespace streams::pipeline {
    class IntPipelineFilterEntries : public virtual Object, public virtual AbstractIntPipeline {
        shared<java::util::function::IntPredicate> _predicate;
        std::optional<int> _next;

    public:
        IntPipelineFilterEntries(const shared<AbstractIntPipeline> &pipeline, const shared<java::util::function::IntPredicate> &predicate);

        bool hasNext() override;

        int next() override;
    };
}
