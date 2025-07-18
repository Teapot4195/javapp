#pragma once

#include <Object.h>

namespace java::util {
    class RandomAccess : public virtual Object {
    public:
        ~RandomAccess() override = default;
    };
}
