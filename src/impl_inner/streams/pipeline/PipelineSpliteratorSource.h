#pragma once

#include <Object.h>
#include <streams/pipeline/AbstractPipeline.h>

namespace streams::pipeline {
    /**
     * @brief Basically an adaptor for spliterators into iterators.
     */
    class PipelineSpliteratorSource final : public virtual Object, public virtual AbstractPipeline {
        shared<java::util::Spliterator> _spliterator;
        std::optional<shared<Object>> _next;
        shared<java::util::function::Consumer> _cons;

    public:
        explicit PipelineSpliteratorSource(const shared<java::util::Spliterator> &spliterator) : _spliterator(spliterator) {}

        bool hasNext() override;

        shared<Object> next() override;

        void acceptNext(const shared<Object> &next);

        void make_cons();

        shared<AbstractPipeline> split() override;
    };
}
