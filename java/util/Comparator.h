#pragma once

#include <Object.h>

namespace java::util {
    class Comparator : public virtual Object {
    public:
        ~Comparator() override = default;

        virtual int compare(shared<Object> o1, shared<Object> o2) = 0;
    };
}
