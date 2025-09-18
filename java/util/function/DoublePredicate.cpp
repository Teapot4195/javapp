#include "DoublePredicate.h"

namespace java::util::function {
    shared<DoublePredicate> DoublePredicate::And(const shared<DoublePredicate> other) {
        return alloc<DoublePredicate>(lambda(const double value) -> bool {
            return self->test(value) && other->test(value);
        });
    }

    shared<DoublePredicate> DoublePredicate::negate() {
        return alloc<DoublePredicate>(lambda(const double value) -> bool {
            return !self->test(value);
        });
    }

    shared<DoublePredicate> DoublePredicate::Or(const shared<DoublePredicate> other) {
        return alloc<DoublePredicate>(lambda(const double value) -> bool {
            return self->test(value) || other->test(value);
        });
    }

    bool DoublePredicate::test(const double value) {
        return inner(value);
    }
}
