#include "OptionalInt.h"

#include <java/util/function/IntConsumer.h>
#include <java/util/function/IntSupplier.h>
#include <java/util/function/Supplier.h>
#include <Runnable.h>
#include <String.h>

namespace java::util {
    shared<OptionalInt> OptionalInt::empty_instance = make();

    OptionalInt::OptionalInt(int value) : inner(value) {}

    OptionalInt::OptionalInt() : inner(std::nullopt) {}

    shared<OptionalInt> OptionalInt::make() {
        assert_once("OptionalInt's empty instance was instantiated more than once.")
        return shared<OptionalInt>(new OptionalInt());
    }

    // TODO: Pool the instances!
    shared<OptionalInt> OptionalInt::make(const int value) {
        return shared<OptionalInt>(new OptionalInt(value));
    }

    shared<OptionalInt> OptionalInt::empty() {
        return empty_instance;
    }

    bool OptionalInt::equals(Object *obj) {
        if (!obj->instanceof<OptionalInt>())
            return false;
        if (this == obj)
            return true;
        const OptionalInt *o = dynamic_cast<OptionalInt*>(obj);
        return inner == o->inner;
    }

    int OptionalInt::getAsInt() const {
        if (inner.has_value())
            return *inner;
        throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION");
    }

    int OptionalInt::hashCode() {
        if (inner.has_value())
            return *inner;
        return 0;
    }

    void OptionalInt::ifPresent(const shared<function::IntConsumer> &action) const {
        if (inner.has_value())
            action->accept(inner.value());
    }

    void OptionalInt::ifPresentOrElse(const shared<function::IntConsumer> &action, const shared<Runnable> &emptyAction) const {
        if (inner.has_value())
            action->accept(inner.value());
        else
            emptyAction->run();
    }

    bool OptionalInt::isEmpty() const {
        return !inner.has_value();
    }

    bool OptionalInt::isPresent() const {
        return inner.has_value();
    }

    shared<OptionalInt> OptionalInt::of(const int value) {
        return make(value);
    }

    int OptionalInt::orElse(int other) const {
        if (inner.has_value())
            return *inner;
        return other;
    }

    int OptionalInt::orElseGet(const shared<function::IntSupplier> &supplier) const {
        if (inner.has_value())
            return *inner;
        return supplier->getAsInt();
    }

    int OptionalInt::orElseThrow() const {
        if (inner.has_value())
            return *inner;
        throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION");
    }

    int OptionalInt::orElseThrow(const shared<function::Supplier> &exceptionSupplier) const {
        if (inner.has_value())
            return *inner;
        throw exceptionSupplier->get();
    }

    shared<stream::IntStream> OptionalInt::stream() const {
#warning not implemented yet!
        throw std::runtime_error("not implemented yet!");
    }

    shared<String> OptionalInt::toString() {
        return alloc<String>(std::format("OptionalInt{{{}}}", inner.has_value() ? std::to_string(*inner) : ""));
    }
}
