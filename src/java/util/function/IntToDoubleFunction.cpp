#include "IntToDoubleFunction.h"

namespace java::util::function {
    double IntToDoubleFunction::applyAsDouble(int value) {
        return inner(value);
    }
}
