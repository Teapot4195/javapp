#include "AbstractIntPipeline.h"

namespace streams::pipeline {
    int AbstractIntPipeline::next() {
        _before->next();
    }

    bool AbstractIntPipeline::hasNext() {
        _before->hasNext();
    }

    shared<AbstractIntPipeline> AbstractIntPipeline::split() {
        return _before->split();
    }

    void AbstractIntPipeline::close() {
        _before->close();
    }
}
