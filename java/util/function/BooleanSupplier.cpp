#include "BooleanSupplier.h"

namespace java::util::function {
    bool BooleanSupplier::getAsBoolean() {
        return inner();
    }
}
