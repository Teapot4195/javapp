#pragma once

#include <Object.h>

namespace java::io {
    class Serializable : public virtual Object {
    public:
        ~Serializable() override = default;
    };
}
