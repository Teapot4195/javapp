#include "DoubleBinaryOperator.h"

namespace java::util::function {
    double DoubleBinaryOperator::applyAsDouble(const double left, const double right) {
        return inner(left, right);
    }
}
