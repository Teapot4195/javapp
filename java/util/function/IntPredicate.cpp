#include "IntPredicate.h"

namespace java::util::function {
    shared<IntPredicate> IntPredicate::And(const shared<IntPredicate> other) {
        return alloc<IntPredicate>([=](const int value) {
            return test(value) && other->test(value);
        });
    }

    shared<IntPredicate> IntPredicate::negate() {
        return alloc<IntPredicate>([=](const int value) {
            return !test(value);
        });
    }

    shared<IntPredicate> IntPredicate::Or(const shared<IntPredicate> other) {
        return alloc<IntPredicate>([=](const int value) {
            return test(value) || other->test(value);
        });
    }

    bool IntPredicate::test(const int value) {
        return inner(value);
    }
}
