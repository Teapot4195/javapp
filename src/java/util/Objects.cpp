#include "Objects.h"

namespace java::util {
    bool Objects::equals(const shared<Object> &a, const shared<Object> &b) {
        return (a == b) || (a != nullptr && a->equals(b));
    }

    bool Objects::isNull(shared<Object> obj) {
        return (obj == nullptr);
    }
}
