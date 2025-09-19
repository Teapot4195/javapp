#pragma once

#include <Object.h>

class Runnable;

namespace java::util {
    namespace function {
        class Supplier;
        class Consumer;
        class Function;
        class Predicate;
    }

    namespace stream {
        class Stream;
    }

    class Optional final : public virtual Object {
        bool hasValue;
        shared<Object> value;

    public:
        ~Optional() override = default;

        Optional();

        Optional(const shared<Object> &value);

        static shared<Optional> empty();

        bool equals(Object *obj) override;

        shared<Optional> filter(const shared<function::Predicate> &predicate);

        shared<Optional> flatMap(const shared<function::Function> &mapper) const;

        shared<Object> get();

        int hashCode() override;

        void ifPresent(const shared<function::Consumer> &action) const;

        void ifPresentOrElse(const shared<function::Consumer> &action, const shared<Runnable> &emptyAction) const;

        bool isEmpty() const;

        bool isPresent() const;

        shared<Optional> map(const shared<function::Function> &mapper) const;

        static shared<Optional> of(shared<Object> value);

        static shared<Optional> ofNullable(shared<Object> value);

        shared<Optional> Or(const shared<function::Supplier> &supplier);

        shared<Object> orElse(shared<Object> other);

        shared<Object> orElseGet(const shared<function::Supplier> &supplier);

        shared<Object> orElseThrow();

        shared<Object> orElseThrow(const shared<function::Supplier> &exceptionSupplier);

        shared<stream::Stream> stream();

        shared<String> toString() override;
    };
}
