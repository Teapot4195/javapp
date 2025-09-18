#include "Comparator.h"

#include "Comparable.h"
#include "function/Function.h"
#include "function/ToDoubleFunction.h"
#include "function/ToIntFunction.h"
#include "function/ToLongFunction.h"

namespace java::util {
    int Comparator::compare(const shared<Object> o1, const shared<Object> o2) {
        if (inner)
            return inner(o1, o2);
        throw std::runtime_error("No function was provided and not a subclass!");
    }

    shared<Comparator> Comparator::comparing(const shared<function::Function> &keyExtractor) {
        return alloc<Comparator>([=](const shared<Object> &o1, const shared<Object> &o2) {
            const auto other1 = keyExtractor->apply(o1);
            const auto other2 = keyExtractor->apply(o2);

            if (!o1->instanceof<Comparable>() || !o2->instanceof<Comparable>())
                throw std::runtime_error("THROW CLASSCASTEXCEPTION: Comparable");
            const auto c1 = std::dynamic_pointer_cast<Comparable>(other1);
            const auto c2 = std::dynamic_pointer_cast<Comparable>(other2);

            return c1->compareTo(c2);
        });
    }

    shared<Comparator> Comparator::comparing(const shared<function::Function> &keyExtractor, const shared<Comparator> &keyComparator) {
        return alloc<Comparator>([=](const shared<Object> &o1, const shared<Object> &o2) {
            const auto other1 = keyExtractor->apply(o1);
            const auto other2 = keyExtractor->apply(o2);

            return keyComparator->compare(other1, other2);
        });
    }

    shared<Comparator> Comparator::comparingDouble(const shared<function::ToDoubleFunction> &keyExtractor) {
        return alloc<Comparator>([=](const shared<Object> &o1, const shared<Object> &o2) {
            const double other = keyExtractor->applyAsDouble(o1);
            const double other2 = keyExtractor->applyAsDouble(o2);

            if (other < other2)
                return -1;
            if (other > other2)
                return 1;
            return 0;
        });
    }

    shared<Comparator> Comparator::comparingInt(const shared<function::ToIntFunction> &keyExtractor) {
        return alloc<Comparator>([=](const shared<Object> &o1, const shared<Object> &o2) {
            const int other = keyExtractor->applyAsInt(o1);
            const int other2 = keyExtractor->applyAsInt(o2);

            if (other < other2)
                return -1;
            if (other > other2)
                return 1;
            return 0;
        });
    }

    shared<Comparator> Comparator::comparingLong(const shared<function::ToLongFunction> &keyExtractor) {
        return alloc<Comparator>([=](const shared<Object> &o1, const shared<Object> &o2) {
            const long long other = keyExtractor->applyAsLong(o1);
            const long long other2 = keyExtractor->applyAsLong(o2);

            if (other < other2)
                return -1;
            if (other > other2)
                return 1;
            return 0;
        });
    }

    shared<Comparator> Comparator::naturalOrder() {
        // TODO: make singleton!
        return alloc<Comparator>([=](const shared<Object> &o1, const shared<Object> &o2) {
            if (!o1->instanceof<Comparable>() || !o2->instanceof<Comparable>())
                throw std::runtime_error("THROW CLASSCASTEXCEPTION: Comparable");
            const auto other1 = std::dynamic_pointer_cast<Comparable>(o1);
            const auto other2 = std::dynamic_pointer_cast<Comparable>(o2);

            return other1->compareTo(other2);
        });
    }

    shared<Comparator> Comparator::nullsFirst(const shared<Comparator> &comparator) {
        return alloc<Comparator>([=](const shared<Object> &o1, const shared<Object> &o2) {
            if (o1 == nullptr && o2 == nullptr)
                return 0;
            if (o1 == nullptr && o2 != nullptr)
                return -1;
            if (o1 != nullptr && o2 == nullptr)
                return 1;
            return comparator->compare(o1, o2);
        });
    }

    shared<Comparator> Comparator::nullsLast(const shared<Comparator> &comparator) {
        return alloc<Comparator>([=](const shared<Object> &o1, const shared<Object> &o2) {
            if (o1 == nullptr && o2 == nullptr)
                return 0;
            if (o1 == nullptr && o2 != nullptr)
                return 1;
            if (o1 != nullptr && o2 == nullptr)
                return -1;
            return comparator->compare(o1, o2);
        });
    }

    shared<Comparator> Comparator::reversed() {
        return alloc<Comparator>(lambda(const shared<Object> &o1, const shared<Object> &o2) {
            return -1 * self->compare(o1, o2);
        });
    }

    shared<Comparator> Comparator::reverseOrder() {
        return naturalOrder()->reversed();
    }

    shared<Comparator> Comparator::thenComparing(const shared<Comparator> other) {
        return alloc<Comparator>(lambda(const shared<Object> &o1, const shared<Object> &o2) {
            const auto first = self->compare(o1, o2);
            return first == 0 ? other->compare(o1, o2) : first;
        });
    }

    shared<Comparator> Comparator::thenComparing(const shared<function::Function> keyExtractor) {
        return thenComparing(comparing(keyExtractor));
    }

    shared<Comparator> Comparator::thenComparing(const shared<function::Function> keyExtractor,
        const shared<Comparator> keyComparator) {
        return thenComparing(comparing(keyExtractor, keyComparator));
    }

    shared<Comparator> Comparator::thenComparingDouble(const shared<function::ToDoubleFunction> keyExtractor) {
        return thenComparing(comparingDouble(keyExtractor));
    }

    shared<Comparator> Comparator::thenComparingInt(const shared<function::ToIntFunction> keyExtractor) {
        return thenComparing(comparingInt(keyExtractor));
    }

    shared<Comparator> Comparator::thenComparingLong(const shared<function::ToLongFunction> keyExtractor) {
        return thenComparing(comparingLong(keyExtractor));
    }
}
