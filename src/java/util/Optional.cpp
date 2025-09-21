#include "Optional.h"

#include "String.h"
#include "Throwable.h"
#include "function/Consumer.h"
#include "function/Function.h"
#include "function/Predicate.h"
#include "function/Supplier.h"

namespace java::util {
    Optional::Optional() : hasValue(false), value(nullptr) {}

    Optional::Optional(const shared<Object> &value) : hasValue(true), value(value) {}

    shared<Optional> Optional::empty() {
        // TODO: make this one return a global object instead!
        return alloc<Optional>();
    }

    bool Optional::equals(Object *obj) {
        if (!obj->instanceof<Optional>())
            return false;
        const auto* other = dynamic_cast<Optional*>(obj);

        if (!hasValue)
            return !other->hasValue;
        return other->value == value;
    }

    shared<Optional> Optional::filter(const shared<function::Predicate> &predicate) {
        if (!hasValue || !predicate->test(value))
            return empty();
        return alloc<Optional>(value);
    }

    shared<Optional> Optional::flatMap(const shared<function::Function> &mapper) const {
        if (!hasValue)
            return empty();
        if (mapper == nullptr)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION");
        return std::dynamic_pointer_cast<Optional>(mapper->apply(value));
    }

    shared<Object> Optional::get() {
        if (hasValue)
            return value;
        throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION");
    }

    int Optional::hashCode() {
        if (hasValue)
            return value->hashCode();
        return 0;
    }

    void Optional::ifPresent(const shared<function::Consumer> &action) const {
        if (!hasValue)
            return;
        if (action == nullptr)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION");
        action->accept(value);
    }

    void Optional::ifPresentOrElse(const shared<function::Consumer> &action, const shared<Runnable> &emptyAction) const {
        if (!hasValue) {
            if (emptyAction == nullptr)
                throw std::runtime_error("THROW NULLPOINTEREXCEPTION");
            action->accept(value);
            return;
        }
        if (action == nullptr)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION");
        action->accept(value);
    }

    bool Optional::isEmpty() const {
        return !hasValue;
    }

    bool Optional::isPresent() const {
        return hasValue;
    }

    shared<Optional> Optional::map(const shared<function::Function> &mapper) const {
        if (!hasValue)
            return empty();
        if (mapper == nullptr)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION");
        return alloc<Optional>(mapper->apply(value));
    }

    shared<Optional> Optional::of(shared<Object> value) {
        if (value == nullptr)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION");
        return alloc<Optional>(value);
    }

    shared<Optional> Optional::ofNullable(shared<Object> value) {
        if (value == nullptr)
            return empty();
        return alloc<Optional>(value);
    }

    shared<Optional> Optional::Or(const shared<function::Supplier> &supplier) {
        if (hasValue)
            return alloc<Optional>(value);
        if (supplier == nullptr)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION");
        return std::dynamic_pointer_cast<Optional>(supplier->get());
    }

    shared<Object> Optional::orElse(shared<Object> other) {
        if (!hasValue)
            return other;
        return value;
    }

    shared<Object> Optional::orElseGet(const shared<function::Supplier> &supplier) {
        if (hasValue)
            return value;
        if (supplier == nullptr)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION");
        return supplier->get();
    }

    shared<Object> Optional::orElseThrow() {
        if (!hasValue)
            throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION");
        return value;
    }

    shared<Object> Optional::orElseThrow(const shared<function::Supplier> &exceptionSupplier) {
        if (hasValue)
            return value;
        if (exceptionSupplier == nullptr)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION");
        throw std::dynamic_pointer_cast<Throwable>(exceptionSupplier->get());
    }

    shared<stream::Stream> Optional::stream() {
#warning NOT IMPLEMENTED!
        throw std::runtime_error("NOT IMPLEMENTED!");
    }

    shared<String> Optional::toString() {
        return alloc<String>(std::format("Optional{{{}}}", hasValue ? value->toString()->get_backing() : "empty"));
    }
}
