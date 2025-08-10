#include "BiPredicate.h"

namespace java::util::function {
    shared<BiPredicate> BiPredicate::And(shared<BiPredicate> other) {
        return alloc<BiPredicate>([=, this](const shared<Object> &t, const shared<Object> &u) -> bool {
            if (!test(t, u))
                return false;
            return other->test(t, u);
        });
    }

    shared<BiPredicate> BiPredicate::negate() {
        return alloc<BiPredicate>([=, this](const shared<Object> &t, const shared<Object> &u) -> bool {
            return !test(t, u);
        });
    }

    shared<BiPredicate> BiPredicate::Or(shared<BiPredicate> other) {
        return alloc<BiPredicate>([=, this](const shared<Object> &t, const shared<Object> &u) -> bool {
            if (test(t, u))
                return true;
            return other->test(t, u);
        });
    }

    bool BiPredicate::test(const shared<Object> t, const shared<Object> u) {
        return inner(t, u);
    }
}
