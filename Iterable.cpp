#include "Iterable.h"

#include "Long.h"
#include "java/util/Iterator.h"
#include "java/util/Spliterator.h"
#include "java/util/function/Consumer.h"

void Iterable::forEach(shared<java::util::function::Consumer> action) {
    const auto it = iterator();
    while (it->hasNext())
        action->accept(it->next());
}

class Iterable_Spliterator_Default : public virtual java::util::Spliterator {
    shared<java::util::Iterator> inner;

public:
    explicit Iterable_Spliterator_Default(Iterable *iterable) : inner(iterable->iterator()) {}
    ~Iterable_Spliterator_Default() override = default;

    int characteristics() override {
        return 0;
    }

    long long estimateSize() override {
        return Long::MAX_VALUE;
    }

    bool tryAdvance(shared<java::util::function::Consumer> action) override {
        if (!inner->hasNext())
            return false;
        action->accept(inner->next());
        return true;
    }

#warning this is potentially not API compliant
    shared<Spliterator> trySplit() override {
        return nullptr;
    }
};

shared<java::util::Spliterator> Iterable::spliterator() {
    return alloc<Iterable_Spliterator_Default>(this);
}
