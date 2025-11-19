#include "DoubleUnaryOperator.h"

namespace java::util::function {
    shared<DoubleUnaryOperator> DoubleUnaryOperator::andThen(shared<DoubleUnaryOperator> after) {
        return alloc<DoubleUnaryOperator>([=, self=pself](const double value) -> double {
            return after->applyAsDouble(self->applyAsDouble(value));
        });
    }

    double DoubleUnaryOperator::applyAsDouble(const double value) {
        return inner(value);
    }

    shared<DoubleUnaryOperator> DoubleUnaryOperator::compose(shared<DoubleUnaryOperator> before) {
        return alloc<DoubleUnaryOperator>([=, self=pself](const double value) -> double {
            return self->applyAsDouble(before->applyAsDouble(value));
        });
    }

    shared<DoubleUnaryOperator> DoubleUnaryOperator::identity() {
        return alloc<DoubleUnaryOperator>([=](const double value) -> double {
            return value;
        });
    }
}
