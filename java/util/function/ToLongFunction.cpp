#include "ToLongFunction.h"

namespace java::util::function {
    long long ToLongFunction::applyAsLong(const shared<Object> value) {
        return inner(value);
    }
}
