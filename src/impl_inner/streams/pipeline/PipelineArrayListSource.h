#pragma once

#include <Object.h>
#include <streams/pipeline/AbstractPipeline.h>

namespace java::util {
    class ArrayList;
}

namespace streams::pipeline {
    class PipelineArrayListSource : public virtual Object, public virtual AbstractPipeline {
        shared<java::util::ArrayList> list;
        size_t ccount = 0;
        size_t lim = 0;
#ifdef JAVAPP_ENABLE_INTERNAL_ASSERTIONS
        bool iteration_started = false;
#endif

    public:
        explicit PipelineArrayListSource(const shared<java::util::ArrayList> &list);

        PipelineArrayListSource(const shared<java::util::ArrayList> &list, size_t start_index, size_t end_index);

        bool hasNext() override;

        shared<Object> next() override;

        shared<AbstractPipeline> split() override;
    };
}
