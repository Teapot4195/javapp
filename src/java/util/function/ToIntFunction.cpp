#include "ToIntFunction.h"

namespace java::util::function {
    int ToIntFunction::applyAsInt(const shared<Object> value) {
        return inner(value);
    }
}
