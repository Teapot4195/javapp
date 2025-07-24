#include "ObjDoubleConsumer.h"

namespace java::util::function {
    void ObjDoubleConsumer::accept(shared<Object> t, double value) {
        inner(t, value);
    }
}
