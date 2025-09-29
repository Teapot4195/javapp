#include "IntUnaryOperator.h"

namespace java::util::function {
    shared<IntUnaryOperator> IntUnaryOperator::andThen(const shared<IntUnaryOperator> after) {
        return alloc<IntUnaryOperator>(lambda(const int i) {
            return after->applyAsInt(self->applyAsInt(i));
        });
    }

    int IntUnaryOperator::applyAsInt(int i) {
        return inner(i);
    }

    shared<IntUnaryOperator> IntUnaryOperator::compose(shared<IntUnaryOperator> before) {
        return alloc<IntUnaryOperator>(lambda(const int i) {
            return self->applyAsInt(before->applyAsInt(i));
        });
    }

    shared<IntUnaryOperator> IntUnaryOperator::identity() {
        return alloc<IntUnaryOperator>([](const int i) {
            return i;
        });
    }
}
