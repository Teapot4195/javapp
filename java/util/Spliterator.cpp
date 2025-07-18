#include "Spliterator.h"

namespace java::util {
    void Spliterator::forEachRemaining(shared<function::Consumer> action) {
        while (tryAdvance(action))
            ;
    }

    shared<Comparator> Spliterator::getComparator() {
        throw std::runtime_error("THROW ILLEGALSTATEEXCEPTION");
    }

    long long Spliterator::getExactSizeIfKnown() {
        return ((this->characteristics() & SIZED) == SIZED) ? estimateSize() : -1;
    }

    bool Spliterator::hasCharacteristics(const int characteristics) {
        return (this->characteristics() & characteristics) == characteristics;
    }
}
