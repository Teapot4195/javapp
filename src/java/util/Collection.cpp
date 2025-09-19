#include "Collection.h"

#include "Iterator.h"
#include "Spliterators.h"
#include "function/Consumer.h"
#include "function/IntFunction.h"
#include "function/Predicate.h"

namespace java::util {
    bool Collection::equals(Object *obj) {
        return Object::equals(obj);
    }

    int Collection::hashCode() {
        return Object::hashCode();
    }

    shared<stream::Stream> Collection::parallelStream() {
        throw std::runtime_error("NOT IMPLEMENTED!");
    }

    bool Collection::removeIf(const shared<function::Predicate> filter) {
        if (filter == nullptr)
            throw std::runtime_error("NULLPTR SHOULD BE THROWN HERE");

        bool flag = false;

        const auto it = this->iterator();
        while (it->hasNext()) {
            if (const auto next = it->next(); filter->test(next)) {
                it->remove();
                flag = true;
            }
        }

        return flag;
    }

    class Spliterator_Collection_Specialization final : public virtual Spliterator {
        shared<Collection> collection;
        shared<Iterator> iterator;

    public:
        explicit Spliterator_Collection_Specialization(const shared<Collection> &obj) : collection(obj) {
            iterator = collection->iterator();
        }

        ~Spliterator_Collection_Specialization() override = default;

        int characteristics() override {
            return SIZED;
        }

        long long estimateSize() override {
            return collection->size();
        }

        bool tryAdvance(const shared<function::Consumer> action) override {
            if (!iterator->hasNext())
                return false;
            action->accept(iterator->next());
            return true;
        }

        shared<Spliterator> trySplit() override {
            return nullptr;
        }
    };

    shared<Spliterator> Collection::spliterator() {
        if (this->size() == 0)
            return Spliterators::emptySpliterator();
        return alloc<Spliterator_Collection_Specialization>(std::dynamic_pointer_cast<Collection>(shared_from_this()));
    }

    shared<stream::Stream> Collection::stream() {
        throw std::runtime_error("NOT IMPLEMENTED!");
    }

    shared<Array<>> Collection::toArray(const shared<function::IntFunction> generator) {
        return toArray(std::dynamic_pointer_cast<Array<>>(generator->apply(this->size())));
    }
}
