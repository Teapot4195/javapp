#pragma once

#include <Object.h>

namespace java::util {
    class Comparator : public virtual Object {
    public:
        ~Comparator() override = default;
    };
}
