#pragma once

#include <Object.h>
#include <streams/pipeline/AbstractPipeline.h>

namespace java::util::function {
    class IntFunction;
}

namespace streams::pipeline {
    class AbstractIntPipeline;
}

namespace streams::pipeline {
    class IntPipelineMapToObj : public virtual Object, public virtual AbstractPipeline {
        shared<AbstractIntPipeline> source_pipeline_;
        shared<java::util::function::IntFunction> conversion_function_;

    public:
        IntPipelineMapToObj(const shared<AbstractIntPipeline> &source_pipeline,
            const shared<java::util::function::IntFunction> &conversion_function);

        bool hasNext() override;

        shared<Object> next() override;

        void close() override;
    };
}
