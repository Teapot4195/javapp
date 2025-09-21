#include "PipelinePeek.h"

#include <java/util/function/Consumer.h>

namespace streams::pipeline {
    PipelinePeek::PipelinePeek(const shared<AbstractPipeline> &before,
                               const shared<java::util::function::Consumer> &consumer) : AbstractPipeline(before), _cons(consumer) {}

    shared<Object> PipelinePeek::next() {
        shared<Object> ret = AbstractPipeline::next();

        _cons->accept(ret);

        return ret;
    }
}
