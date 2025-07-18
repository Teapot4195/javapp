#include "UnaryOperator.h"

namespace java::util::function {
    shared<UnaryOperator> UnaryOperator::identity() {
        return alloc<UnaryOperator>([&](shared<Object> arg) {
            return arg;
        });
    }
}
