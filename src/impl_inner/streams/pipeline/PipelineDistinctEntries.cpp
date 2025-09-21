#include "PipelineDistinctEntries.h"

#include <SwissTableHashSet.h>

namespace streams::pipeline {
    PipelineDistinctEntries::PipelineDistinctEntries(const shared<AbstractPipeline> &pipeline) :
                AbstractPipeline(pipeline), _hash_set(new SwissTableHashSet()) {}

    PipelineDistinctEntries::~PipelineDistinctEntries() {
        delete _hash_set;
    }

    bool PipelineDistinctEntries::hasNext() {
        while (_before->hasNext()) {
            auto next = _before->next();
            if (_hash_set->table_get(next))
                continue;
            _next = next;
            _hash_set->table_insert_or_set(next);
            return true;
        }

        return false;
    }

    shared<Object> PipelineDistinctEntries::next() {
        if (!hasNext()) // fill the _next buffer;
            throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION!");

        auto next = _next.value();
        _next.reset();
        return next;
    }
}
