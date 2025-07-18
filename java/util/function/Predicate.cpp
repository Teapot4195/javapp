#include "Predicate.h"

#include <utility>

#include "java/util/Objects.h"

namespace java::util::function {
    shared<Predicate> Predicate::And(const shared<Predicate> other) {
        return alloc<Predicate>([&](const shared<Object>& value) -> bool {
            if (!test(value))
                return false;
            return other->test(value);
        });
    }

    shared<Predicate> Predicate::isEqual(const shared<Object> &targetRef) {
        return alloc<Predicate>([&](const shared<Object>& object) -> bool {
            return (targetRef == nullptr) ? Objects::isNull : targetRef->equals(object);
        });
    }

    shared<Predicate> Predicate::Not(const shared<Predicate> &target) {
        return target->negate();
    }

    shared<Predicate> Predicate::negate() {
        return alloc<Predicate>([&](const shared<Object>& value) -> bool {
            return !test(value);
        });
    }

    shared<Predicate> Predicate::Or(const shared<Predicate> other) {
        return alloc<Predicate>([&](const shared<Object>& value) -> bool {
            if (test(value))
                return true;
            return other->test(value);
        });
    }

    bool Predicate::test(shared<Object> value) {
        return inner(std::move(value));
    }
}
