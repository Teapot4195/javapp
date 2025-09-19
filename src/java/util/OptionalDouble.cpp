#include "OptionalDouble.h"

#include <String.h>
#include <Throwable.h>
#include "function/DoubleConsumer.h"
#include "function/DoubleSupplier.h"
#include "function/Supplier.h"

namespace java::util {
    OptionalDouble::OptionalDouble(const double value): hasValue(true), value(value) {}

    OptionalDouble::OptionalDouble(): hasValue(false), value(0) {}

    shared<OptionalDouble> OptionalDouble::empty() {
        return alloc<OptionalDouble>();
    }

    bool OptionalDouble::equals(Object *obj) {
        if (!obj->instanceof<OptionalDouble>())
            return false;
        const auto other = dynamic_cast<OptionalDouble *>(obj);

        if (!hasValue) {
            return !other->hasValue;
        }
        if (other->hasValue)
            return other->value == value;
        return false;
    }

    double OptionalDouble::getAsDouble() const {
        if (!hasValue)
            throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION");
        return value;
    }

    int OptionalDouble::hashCode() {
        if (hasValue)
            return value;
        return 0;
    }

    void OptionalDouble::ifPresent(const shared<function::DoubleConsumer> &action) const {
        if (!hasValue)
            throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION");
        action->accept(value);
    }

    void OptionalDouble::ifPresentOrElse(shared<function::DoubleConsumer>, shared<Runnable> emptyAction) {

    }

    bool OptionalDouble::isEmpty() const {
        return !hasValue;
    }

    bool OptionalDouble::isPresent() const {
        return hasValue;
    }

    shared<OptionalDouble> OptionalDouble::of(double value) {
        return alloc<OptionalDouble>(value);
    }

    double OptionalDouble::orElse(const double other) const {
        if (hasValue)
            return value;
        return other;
    }

    double OptionalDouble::orElseGet(const shared<function::DoubleSupplier> &supplier) const {
        if (hasValue)
            return value;
        return supplier->getAsDouble();
    }

    double OptionalDouble::orElseThrow() {
        if (hasValue)
            return value;
        throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION");
    }

    double OptionalDouble::orElseThrow(const shared<function::Supplier> &exceptionSupplier) const {
        if (hasValue)
            return value;
        throw std::dynamic_pointer_cast<Throwable>(exceptionSupplier->get());
    }

    shared<stream::DoubleStream> OptionalDouble::stream() {
#warning NOT IMPLEMENTED (NON COMPLIANT)
        throw std::runtime_error("not compliant");
    }

    shared<String> OptionalDouble::toString() {
        return alloc<String>(hasValue ? std::format("{}", value) : "empty");
    }
}
