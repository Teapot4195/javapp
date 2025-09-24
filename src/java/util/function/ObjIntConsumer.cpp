#include "ObjIntConsumer.h"

namespace java::util::function {
    void ObjIntConsumer::accept(shared<Object> t, int value) {
        inner(t, value);
    }
}
