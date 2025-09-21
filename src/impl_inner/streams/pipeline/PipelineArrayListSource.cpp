#include "PipelineArrayListSource.h"

#include <java/util/ArrayList.h>

namespace streams::pipeline {
    PipelineArrayListSource::PipelineArrayListSource(const shared<java::util::ArrayList> &list): list(list), lim(list->size()) {}

    PipelineArrayListSource::PipelineArrayListSource(const shared<java::util::ArrayList> &list,
        const size_t start_index, const size_t end_index) : list(list), ccount(start_index), lim(end_index) {
    }

    bool PipelineArrayListSource::hasNext() {
        return ccount < lim;
    }

    shared<Object> PipelineArrayListSource::next() {
#ifdef JAVAPP_ENABLE_INTERNAL_ASSERTIONS
        iteration_started = true;
#endif
        return list->get(ccount++);
    }

    shared<AbstractPipeline> PipelineArrayListSource::split() {
#ifdef JAVAPP_ENABLE_INTERNAL_ASSERTIONS
        if (iteration_started)
            panic("pipeline was split after iteration began!");
#endif
        // wrap the new pipeline into a new list source.
        auto split_point = list->size() / 2;
        ccount = split_point;
        return alloc<PipelineArrayListSource>(list, 0, split_point);
    }
}
