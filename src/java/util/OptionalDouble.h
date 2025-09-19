#pragma once

#include <Object.h>

class Runnable;

namespace java::util {
    namespace function {
        class DoubleConsumer;
        class DoubleSupplier;
        class Supplier;
    }

    namespace stream {
        class DoubleStream;
    }

    class OptionalDouble final : public virtual Object {
        bool hasValue;
        double value;

    public:
        ~OptionalDouble() override = default;

        explicit OptionalDouble(double value);

        OptionalDouble();

        static shared<OptionalDouble> empty();

        bool equals(Object *obj) override;

        DEFINE_SHARED_EQUALS

        double getAsDouble() const;

        int hashCode() override;

        void ifPresent(const shared<function::DoubleConsumer> &action) const;

        void ifPresentOrElse(shared<function::DoubleConsumer>, shared<Runnable> emptyAction);

        bool isEmpty() const;

        bool isPresent() const;

        static shared<OptionalDouble> of(double value);

        double orElse(double other) const;

        double orElseGet(const shared<function::DoubleSupplier> &supplier) const;

        double orElseThrow();

        double orElseThrow(const shared<function::Supplier> &exceptionSupplier) const;

        shared<stream::DoubleStream> stream();

        shared<String> toString() override;
    };
}
