#include "PipelineSkip.h"

namespace streams::pipeline {
    PipelineSkip::PipelineSkip(const shared<AbstractPipeline> &before, size_t count) : AbstractPipeline(before), _skip(count) {}

    shared<Object> PipelineSkip::next() {
        if (!_skipped)
            perform_skip();
        return AbstractPipeline::next();
    }

    bool PipelineSkip::hasNext() {
        if (!_skipped)
            perform_skip();
        return AbstractPipeline::hasNext();
    }

    void PipelineSkip::perform_skip() {
        for (int i = 0; i < _skip && _before->hasNext(); i++)
            _before->next();

        _skipped = true;
    }
}
