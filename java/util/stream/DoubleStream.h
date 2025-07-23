#pragma once

#include "BaseStream.h"

namespace java::util::stream {
    class DoubleStream : public virtual Object, public virtual BaseStream {
    public:
        ~DoubleStream() override = default;
    };
}
