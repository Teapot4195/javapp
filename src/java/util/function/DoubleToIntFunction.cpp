#include "DoubleToIntFunction.h"

namespace java::util::function {
    int DoubleToIntFunction::applyasInt(const double value) {
        return inner(value);
    }
}
