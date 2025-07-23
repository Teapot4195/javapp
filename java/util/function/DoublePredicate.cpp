#include "DoublePredicate.h"

namespace java::util::function {
    shared<DoublePredicate> DoublePredicate::And(const shared<DoublePredicate> other) {
        return alloc<DoublePredicate>([=](const double value) -> bool {
            return test(value) && other->test(value);
        });
    }

    shared<DoublePredicate> DoublePredicate::negate() {
        return alloc<DoublePredicate>([=](const double value) -> bool {
            return !test(value);
        });
    }

    shared<DoublePredicate> DoublePredicate::Or(const shared<DoublePredicate> other) {
        return alloc<DoublePredicate>([=](const double value) -> bool {
            return test(value) || other->test(value);
        });
    }

    bool DoublePredicate::test(const double value) {
        return inner(value);
    }
}
