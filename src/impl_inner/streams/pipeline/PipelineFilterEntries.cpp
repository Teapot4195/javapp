#include "PipelineFilterEntries.h"

#include <java/util/function/Predicate.h>

namespace streams::pipeline {
    PipelineFilterEntries::PipelineFilterEntries(const shared<AbstractPipeline> &parent,
                                                 const shared<java::util::function::Predicate> &predicate)
        : AbstractPipeline(parent), _predicate(predicate) {}

    bool PipelineFilterEntries::hasNext() {
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

    shared<Object> PipelineFilterEntries::next() {
        if (!hasNext()) // fill the _next buffer;
            throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION!");

        auto next = _next.value();
        _next.reset();
        return next;
    }
}
