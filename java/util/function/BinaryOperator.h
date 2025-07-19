#pragma once

#include <Object.h>

#include "BiFunction.h"

namespace java::util {
    class Comparator;
}

namespace java::util::function {

    class BinaryOperator : public virtual Object, public virtual BiFunction {
    public:
        ~BinaryOperator() override = default;

        using BiFunction::BiFunction;

        static shared<BinaryOperator> maxBy(const shared<Comparator> &comparator);

        static shared<BinaryOperator> minBy(const shared<Comparator> &comparator);
    };

}
