#pragma once

#include <streams/pipeline/AbstractPipeline.h>

namespace java::util::function {
    class Predicate;
}

namespace streams::pipeline {
    class PipelineFilterEntries : public virtual Object, public virtual AbstractPipeline {
        shared<java::util::function::Predicate> _predicate;
        std::optional<shared<Object>> _next;

    public:
        PipelineFilterEntries(const shared<AbstractPipeline> &parent, const shared<java::util::function::Predicate> &predicate);

        bool hasNext() override;

        shared<Object> next() override;
    };
}
