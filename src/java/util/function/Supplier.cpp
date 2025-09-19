#include "Supplier.h"

namespace java::util::function {
    shared<Object> Supplier::get() {
        return inner();
    }
}
