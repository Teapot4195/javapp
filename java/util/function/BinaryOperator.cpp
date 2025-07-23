#include "BinaryOperator.h"

#include "java/util/Comparator.h"

namespace java::util::function {
    shared<BinaryOperator> BinaryOperator::maxBy(const shared<Comparator> &comparator) {
        return alloc<BinaryOperator>([=](shared<Object> o1, shared<Object> o2) {
            if (comparator->compare(o1, o2) < 0)
                return o2;
            return o1;
        });
    }

    shared<BinaryOperator> BinaryOperator::minBy(const shared<Comparator> &comparator) {
        return alloc<BinaryOperator>([=](shared<Object> o1, shared<Object> o2) {
            if (comparator->compare(o1, o2) > 0)
                return o2;
            return o1;
        });
    }
}
