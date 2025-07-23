#pragma once

#include "BaseStream.h"
#include "java/util/function/DoubleConsumer.h"

namespace java::util::stream {
    class DoubleStream : public virtual Object, public virtual BaseStream {
    public:
        ~DoubleStream() override = default;

        class DoubleMapMultiConsumer;
        class Builder;
    };

    class DoubleStream::DoubleMapMultiConsumer : public virtual Object {
        std::function<void(double, shared<function::DoubleConsumer>)> inner;

    public:
        ~DoubleMapMultiConsumer() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F function type
         * @param func the inner function
         */
        template <typename F>
        DoubleMapMultiConsumer(F&& func) : inner(std::forward<F>(func)) {}

        virtual void accept(double value, shared<function::DoubleConsumer> dc);
    };

    class DoubleStream::Builder : public virtual Object, public virtual function::DoubleConsumer {
    public:
        ~Builder() override = default;

        void accept(double value) override = 0;

        virtual shared<Builder> add(double t);

        virtual shared<DoubleStream> build() = 0;
    };
}
