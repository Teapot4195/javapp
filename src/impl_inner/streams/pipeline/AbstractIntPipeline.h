#pragma once

#include <Object.h>

namespace streams::pipeline {
    class AbstractIntPipeline : public virtual Object {
    protected:
        shared<AbstractIntPipeline> _before;

    public:
        AbstractIntPipeline() : _before(nullptr) {}

        explicit AbstractIntPipeline(const shared<AbstractIntPipeline> &before) : _before(before) {}

        virtual int next();

        virtual bool hasNext();

        virtual shared<AbstractIntPipeline> split();

        virtual void close();
    };
}
