#include "IntToLongFunction.h"

namespace java::util::function {
    long long IntToLongFunction::applyAsDouble(int value) {
        return inner(value);
    }
}
