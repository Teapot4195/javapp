#include "IntPipelinePeek.h"

#include <java/util/function/IntConsumer.h>

namespace streams::pipeline {
    IntPipelinePeek::IntPipelinePeek(shared<AbstractIntPipeline> before,
        shared<java::util::function::IntConsumer> peeker) : AbstractIntPipeline(before), peeker_(peeker) {}

    int IntPipelinePeek::next() {
        auto result = AbstractIntPipeline::next();

        peeker_->accept(result);

        return result;
    }
}
