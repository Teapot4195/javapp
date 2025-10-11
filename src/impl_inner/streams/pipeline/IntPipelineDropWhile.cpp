#include "IntPipelineDropWhile.h"

#include <java/util/function/IntPredicate.h>

namespace streams::pipeline {
    IntPipelineDropWhile::IntPipelineDropWhile(shared<AbstractIntPipeline> pipeline,
                                               shared<java::util::function::IntPredicate> predicate)
        : AbstractIntPipeline(pipeline), predicate(predicate) {}

    bool IntPipelineDropWhile::hasNext() {
        if (!dropped) {
            while (_before->hasNext()) {
                auto next = _before->next();
                if (!predicate->test(next)) {
                    _next = next;
                    break;
                }
            }

            dropped = true;

            return true;
        }

        return _before->hasNext();
    }

    int IntPipelineDropWhile::next() {
        if (!dropped)
            hasNext(); // perform dropping

        if (_next.has_value()) {
            auto next = _next.value();
            _next.reset();
            return next;
        }

        return _before->next();
    }
}
