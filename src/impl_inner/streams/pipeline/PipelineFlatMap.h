#pragma once

#include <streams/pipeline/AbstractPipeline.h>

namespace java::util::stream {
    class Stream;
}

namespace streams::pipeline {
    class PipelineFlatMap : public virtual Object, public virtual AbstractPipeline {
        shared<java::util::function::Function> mapper;
        shared<java::util::stream::Stream> stream;
        shared<Iterator> iterator_;
        std::optional<shared<Object>> _next;

    public:
        PipelineFlatMap(const shared<AbstractPipeline> &before, const shared<java::util::function::Function> &mapper);

        bool hasNext() override;

        shared<Object> next() override;

        bool fillNextBuffer();
    };
}
