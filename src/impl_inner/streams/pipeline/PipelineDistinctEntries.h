#pragma once

#include <streams/pipeline/AbstractPipeline.h>

class SwissTableHashSet;

namespace streams::pipeline {
    class PipelineDistinctEntries : public virtual Object, public virtual AbstractPipeline {
        SwissTableHashSet* _hash_set;
        std::optional<shared<Object>> _next;

    public:
        explicit PipelineDistinctEntries(const shared<AbstractPipeline> &pipeline);

        ~PipelineDistinctEntries() override;

        bool hasNext() override;

        shared<Object> next() override;
    };
}
