#pragma once

#include <Object.h>

#include "Spliterator.h"

namespace java::util {
    class Spliterators : public virtual Object {
    public:
        ~Spliterators() override = default;

        /*MISSING FUNTIONS GO HERE*/

        static shared<Spliterator> emptySpliterator();
    };
}
