#include "DoubleFunction.h"

namespace java::util::function {
    shared<Object> DoubleFunction::apply(const double value) {
        return inner(value);
    }
}
