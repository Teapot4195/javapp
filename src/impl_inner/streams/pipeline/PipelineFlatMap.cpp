#include "PipelineFlatMap.h"

#include <java/util/Optional.h>
#include <java/util/Spliterator.h>
#include <java/util/function/Consumer.h>
#include <java/util/function/Function.h>
#include <java/util/stream/Stream.h>

namespace streams::pipeline {
    PipelineFlatMap::PipelineFlatMap(const shared<AbstractPipeline> &before,
        const shared<java::util::function::Function> &mapper) : AbstractPipeline(before), mapper(mapper) {}

    bool PipelineFlatMap::hasNext() {
        return _next.has_value() ||  fillNextBuffer();
    }

    shared<Object> PipelineFlatMap::next() {
        if (!_next.has_value())
            fillNextBuffer();

        auto next = _next.value();
        _next.reset();
        return next;
    }

    bool PipelineFlatMap::fillNextBuffer() {
        if (!_cons)
            _cons = alloc<java::util::function::Consumer>([&, self=wself](const shared<Object>& obj) {
                self.lock()->_next = alloc<java::util::Optional>(obj);
            });

        if (!_before->hasNext())
            return false;
        while (_before->hasNext()) {
            if (!splitr)
                splitr = std::dynamic_pointer_cast<java::util::stream::Stream>(mapper->apply(_before->next()))->spliterator();
            if (splitr->tryAdvance(_cons))
                return true;
            splitr = nullptr;
        }
        return false;
    }
}
