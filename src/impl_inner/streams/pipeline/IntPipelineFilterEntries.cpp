#include "IntPipelineFilterEntries.h"

#include <java/util/function/IntPredicate.h>

namespace streams::pipeline {
    IntPipelineFilterEntries::IntPipelineFilterEntries(const shared<AbstractIntPipeline> &pipeline,
        const shared<java::util::function::IntPredicate> &predicate) : AbstractIntPipeline(pipeline), _predicate(predicate) {}

    bool IntPipelineFilterEntries::hasNext() {
        if (_next.has_value())
            return true;

        while (_before->hasNext()) {
            auto next = _before->next();
            if (!_predicate->test(next))
                continue;
            _next = next;
            return true;
        }

        return false;
    }

    int IntPipelineFilterEntries::next() {
        if (!hasNext()) // fill the _next buffer
            throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION!");

        auto next = _next.value();
        _next.reset();
        return next;
    }
}
