#pragma once

#include <Object.h>

#include "BaseStream.h"

namespace java::util::stream {
    class IntStream : public virtual Object, public virtual BaseStream {
    public:
        ~IntStream() override = default;
    };
}
