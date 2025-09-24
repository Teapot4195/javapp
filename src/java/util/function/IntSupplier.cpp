#include "IntSupplier.h"

namespace java::util::function {
    int IntSupplier::getAsInt() {
        return inner();
    }
}
