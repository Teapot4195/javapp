#include "IntPipelineDistinctEntries.h"

namespace streams::pipeline {
    IntPipelineDistinctEntries::IntPipelineDistinctEntries(const shared<AbstractIntPipeline> &pipeline)
        : AbstractIntPipeline(pipeline) {}

    IntPipelineDistinctEntries::~IntPipelineDistinctEntries() = default;

    bool IntPipelineDistinctEntries::hasNext() {
        while (_before->hasNext()) {
            auto next = _before->next();
            if (_hash_set.contains(next))
                continue;
            _hash_set.insert(next);
            _next = next;
            return true;
        }

        return false;
    }

    int IntPipelineDistinctEntries::next() {
        if (!hasNext()) // fill _next
            throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION!");

        const int next = _before->next();
        _next.reset();
        return next;
    }
}
