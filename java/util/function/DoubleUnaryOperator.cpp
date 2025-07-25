#include "DoubleUnaryOperator.h"

namespace java::util::function {
    shared<DoubleUnaryOperator> DoubleUnaryOperator::andThen(shared<DoubleUnaryOperator> after) {
        return alloc<DoubleUnaryOperator>([=](const double value) -> double {
            return after->applyAsDouble(applyAsDouble(value));
        });
    }

    double DoubleUnaryOperator::applyAsDouble(const double value) {
        return inner(value);
    }

    shared<DoubleUnaryOperator> DoubleUnaryOperator::compose(shared<DoubleUnaryOperator> before) {
        return alloc<DoubleUnaryOperator>([=](const double value) -> double {
            return applyAsDouble(before->applyAsDouble(value));
        });
    }

    shared<DoubleUnaryOperator> DoubleUnaryOperator::identity() {
        return alloc<DoubleUnaryOperator>([=](const double value) -> double {
            return value;
        });
    }
}
