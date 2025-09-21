#pragma once

#include "AbstractPipeline.h"

namespace streams::pipeline {
    class PipelineSkip : public virtual Object, public virtual AbstractPipeline {
        const size_t _skip;
        bool _skipped = false;

    public:
        PipelineSkip(const shared<AbstractPipeline> &before, size_t count);

        shared<Object> next() override;

        bool hasNext() override;

        void perform_skip();
    };
}
