#include "ToDoubleFunction.h"

namespace java::util::function {
    double ToDoubleFunction::applyAsDouble(const shared<Object> value) {
        return inner(value);
    }
}
