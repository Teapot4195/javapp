#include "DoubleToLongFunction.h"

namespace java::util::function {
    long long DoubleToLongFunction::applyAsLong(const double value) {
        return inner(value);
    }
}
