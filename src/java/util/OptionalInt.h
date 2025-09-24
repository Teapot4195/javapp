#pragma once

#include <Object.h>

class Runnable;

namespace java::util::function {
    class Supplier;
    class IntSupplier;
    class IntConsumer;
}

namespace java::util {
    namespace stream {
        class IntStream;
    }

    // NOTE: most virtuals are elided here as this is final, performance improvements can be had this way.
    class OptionalInt final : public virtual Object {
        std::optional<int> inner;

        explicit OptionalInt(int value);

        OptionalInt();

        static shared<OptionalInt> make();

    public:

        static shared<OptionalInt> make(int value);

        static shared<OptionalInt> empty_instance;

        static shared<OptionalInt> empty();

        bool equals(Object *obj) override;

        DEFINE_SHARED_EQUALS

        int getAsInt() const;

        int hashCode() override;

        void ifPresent(const shared<function::IntConsumer> &action) const;

        void ifPresentOrElse(const shared<function::IntConsumer> &action, const shared<Runnable> &emptyAction) const;

        bool isEmpty() const;

        bool isPresent() const;

        static shared<OptionalInt> of(int value);

        int orElse(int other) const;

        int orElseGet(const shared<function::IntSupplier> &supplier) const;

        int orElseThrow() const;

        int orElseThrow(const shared<function::Supplier> &exceptionSupplier) const;

        shared<stream::IntStream> stream() const;

        shared<String> toString() override;
    };
}
