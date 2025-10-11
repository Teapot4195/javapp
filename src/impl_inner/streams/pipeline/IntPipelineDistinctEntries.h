#pragma once

#include <unordered_set>
#include <streams/pipeline/AbstractIntPipeline.h>

namespace streams::pipeline {
    class IntPipelineDistinctEntries : public virtual Object, public virtual AbstractIntPipeline {
        std::unordered_set<int> _hash_set;
        std::optional<int> _next;

    public:
        explicit IntPipelineDistinctEntries(const shared<AbstractIntPipeline>& pipeline);

        ~IntPipelineDistinctEntries() override;

        bool hasNext() override;

        int next() override;
    };
}
