#pragma once

#include <Object.h>
#include <java/util/Iterator.h>

namespace java::util::function {
    class Function;
}

namespace java::util::function {
    class Predicate;
}

namespace java::util {
    class Spliterator;
}

namespace streams::pipeline {
    class PipelineSpliteratorSource;
}

namespace streams::pipeline {
    class AbstractPipeline : public virtual Object, public virtual java::util::Iterator {
    protected:
        shared<AbstractPipeline> _before;

    public:
        AbstractPipeline() : _before(nullptr) {}

        explicit AbstractPipeline(const shared<AbstractPipeline> &before) : _before(before) {};

        shared<Object> next() override;

        bool hasNext() override;

        static shared<AbstractPipeline> fromSpliterator(shared<java::util::Spliterator> spliterator);

        static shared<AbstractPipeline> transformDistinct(const shared<AbstractPipeline> &pipeline);

        static shared<AbstractPipeline> transformFilter(const shared<AbstractPipeline> &pipeline, const shared<java::util::function::Predicate> &predicate);

        static shared<AbstractPipeline> transformFlatMap(const shared<AbstractPipeline> &pipeline, const shared<java::util::function::Function> &mapper);

        static shared<AbstractPipeline> transformLimit(const shared<AbstractPipeline> &pipeline, long long limit);

        static shared<AbstractPipeline> transformMap(const shared<AbstractPipeline> &pipeline, const shared<java::util::function::Function> &mapper);

        static shared<AbstractPipeline> transformPeek(const shared<AbstractPipeline> &pipeline, const shared<java::util::function::Consumer> &mapper);

        static shared<AbstractPipeline> transformSkip(const shared<AbstractPipeline> &pipeline, long long count);

        virtual shared<AbstractPipeline> split();

        virtual void close();
    };
}
