#include "PipelineSpliteratorSource.h"

#include <java/util/function/Consumer.h>
#include <java/util/Spliterator.h>

namespace streams::pipeline {
    bool PipelineSpliteratorSource::hasNext() {
        if (_next.has_value()) {
            return true;
        }

        make_cons();
        if (!_spliterator->tryAdvance(_cons))
            return false;
        return true;
    }

    shared<Object> PipelineSpliteratorSource::next() {
        if (!_next.has_value()) {
            make_cons();
            if (!_spliterator->tryAdvance(_cons)) {
                throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION!");
            }
        }

        shared<Object> result = _next.value();
        _next.reset();
        return result;
    }

    void PipelineSpliteratorSource::acceptNext(const shared<Object> &next) {
        _next = next;
    }

    void PipelineSpliteratorSource::make_cons() {
        if (!_cons) {
            _cons = alloc<java::util::function::Consumer>([=, self=pself](const shared<Object>& obj) {
                self->acceptNext(obj);
            });
        }
    }

    shared<AbstractPipeline> PipelineSpliteratorSource::split() {
#ifdef JAVAPP_ENABLE_INTERNAL_ASSERTIONS
        if (_next.has_value())
            panic("pipeline was split after iteration began!");
#endif
        // wrap the new pipeline into a spliterator.
        return alloc<PipelineSpliteratorSource>(_spliterator->trySplit());
    }
}
