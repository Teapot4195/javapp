#include "IntFunction.h"

namespace java::util::function {
    shared<Object> IntFunction::apply(const int value) {
        return inner(value);
    }
}
