#include "DoubleSupplier.h"

namespace java::util::function {
    double DoubleSupplier::getAsDouble() {
        return inner();
    }
}
