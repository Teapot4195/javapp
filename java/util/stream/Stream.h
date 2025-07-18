#pragma once

#include "BaseStream.h"
#include <Object.h>

namespace java::util::stream {
    class Stream : public virtual Object, public virtual BaseStream {
    public:
        ~Stream() override = default;
    };

}
