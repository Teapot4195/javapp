#pragma once
#include <streams/pipeline/AbstractPipeline.h>

namespace streams::pipeline {
    class PipelineLimit : public virtual Object, public virtual AbstractPipeline {
        const size_t _limit;
        size_t count;

    public:
        PipelineLimit(const shared<AbstractPipeline> &before, size_t limit);

        bool hasNext() override;

        shared<Object> next() override;
    };
}
