#include "IntBinaryOperator.h"

namespace java::util::function {
    int IntBinaryOperator::applyAsInt(int left, int right) {
        return inner(left, right);
    }
}
