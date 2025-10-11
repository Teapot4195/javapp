#pragma once
#include <Object.h>
#include <streams/pipeline/AbstractPipeline.h>

namespace streams::pipeline {
    class AbstractIntPipeline;

    class IntPipelineBoxer : public virtual Object, public virtual AbstractPipeline {
        shared<AbstractIntPipeline> source_;

    public:
        explicit IntPipelineBoxer(const shared<AbstractIntPipeline> &source);

        void forEachRemaining(shared<java::util::function::Consumer> action) override;

        shared<Object> next() override;

        bool hasNext() override;

        void close() override;

        ~IntPipelineBoxer() override;
    };
}
