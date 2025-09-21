#include "PipelineLimit.h"

namespace streams::pipeline {
    PipelineLimit::PipelineLimit(const shared<AbstractPipeline> &before, const size_t limit) : AbstractPipeline(before),
        _limit(limit), count(0) {
    }

    bool PipelineLimit::hasNext() {
        if (AbstractPipeline::next() && count < _limit)
            return true;
        return false;
    }

    shared<Object> PipelineLimit::next() {
        count++;
        if (count > _limit)
            throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION!");

        return AbstractPipeline::next();
    }
}
