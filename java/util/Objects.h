#pragma once

#include <Object.h>

namespace java::util {
    class Objects : public virtual Object {
    public:
        ~Objects() override = default;

        static bool equals(const shared<Object> &a, const shared<Object> &b);

        /*stuff in between*/

        static bool isNull(shared<Object> obj);
    };

}
