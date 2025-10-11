#pragma once

#include <Object.h>
#include <streams/pipeline/AbstractIntPipeline.h>

namespace java::util::function {
    class IntConsumer;
}

namespace streams::pipeline {
    class IntPipelinePeek : public virtual Object, public virtual AbstractIntPipeline {
        shared<java::util::function::IntConsumer> peeker_;

    public:
        IntPipelinePeek(shared<AbstractIntPipeline> before, shared<java::util::function::IntConsumer> peeker);

        int next() override;
    };
}
