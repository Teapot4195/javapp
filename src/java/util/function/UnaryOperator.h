#pragma once

#include <Object.h>

#include "Function.h"

namespace java::util::function {
    class UnaryOperator : public virtual Object, public virtual Function {
    public:
        ~UnaryOperator() override = default;

        using Function::Function;

        static shared<UnaryOperator> identity();
    };

}
