#include "Set.h"

#include <set>

#include <java/util/Iterator.h>
#include <java/util/Spliterator.h>
#include <java/util/function/Consumer.h>

namespace java::util {
    class Set_Immutable_Specialization final : public virtual Object, public virtual Set {
        std::set<shared<Object>> set;

        friend class Set_Immutable_Specialization_Iterator_Specialization;

    public:
        ~Set_Immutable_Specialization() override = default;

        explicit Set_Immutable_Specialization(std::set<shared<Object>>&& set) : set(std::move(set)) {}

        bool add(shared<Object> e) override {
            throw std::runtime_error("NOT IMPLEMENTED EXCEPTION!!!!");
        }

        bool addAll(shared<Collection> c) override {
            throw std::runtime_error("NOT IMPLEMENTED EXCEPTION!!!!");
        }

        void clear() override {
            throw std::runtime_error("NOT IMPLEMENTED EXCEPTION!!!!");
        }

        bool contains(const shared<Object> o) override {
            return set.contains(o);
        }

        bool containsAll(const shared<Collection> c) override {
            const auto it = c->iterator();
            while (it->hasNext()) {
                if (!set.contains(it->next()))
                    return false;
            }
            return true;
        }

        bool isEmpty() override {
            return set.empty();
        }

        class Set_Immutable_Specialization_Iterator_Specialization final : public virtual Object, public virtual Iterator {
            shared<Set_Immutable_Specialization> set;
            std::set<shared<Object>>::const_iterator it;

        public:
            ~Set_Immutable_Specialization_Iterator_Specialization() override = default;

            explicit Set_Immutable_Specialization_Iterator_Specialization(const shared<Set_Immutable_Specialization> &set) : set(set), it(set->set.cbegin()) {}

            bool hasNext() override {
                return it != set->set.cend();
            }

            shared<Object> next() override {
                return *(it++);
            }
        };

        shared<Iterator> iterator() override {
            return alloc<Set_Immutable_Specialization_Iterator_Specialization>(std::dynamic_pointer_cast<Set_Immutable_Specialization>(shared_from_this()));
        }

        bool remove(shared<Object> o) override {
            throw std::runtime_error("NOT IMPLEMENTED EXCEPTION!!!!");
        }

        bool removeAll(shared<Collection> c) override {
            throw std::runtime_error("NOT IMPLEMENTED EXCEPTION!!!!");
        }

        bool retainAll(shared<Collection> c) override {
            throw std::runtime_error("NOT IMPLEMENTED EXCEPTION!!!!");
        }

        int size() override {
            return static_cast<int>(set.size());
        }

        // TODO: neither of these are optimal!
        shared<Array<>> toArray() override {
            std::vector v(set.begin(), set.end());
            return alloc<Array<>>(v);
        }

        // TODO: neither of these are optimal!
        // TODO: we should probably use whatever they gave us too?
        shared<Array<>> toArray(shared<Array<>> a) override {
            std::vector v(set.begin(), set.end());
            return alloc<Array<>>(v);
        }
    };

    // TODO: check if this is an unmodifiable Set and if so just return it?
    shared<Set> Set::copyOf(const shared<Collection>& coll) {
        std::set<shared<Object>> set;

        if (coll == nullptr)
            throw std::runtime_error("NULLPOINTEREXCEPTION!!!!");

        const auto it = coll->iterator();
        while (it->hasNext()) {
            auto element = it->next();
            if (element == nullptr)
                throw std::runtime_error("NULLPOINTEREXCEPTION!!!!");
            set.insert(element);
        }

        return alloc<Set_Immutable_Specialization>(std::move(set));
    }

    shared<Set> Set::of(const std::vector<shared<Object>>& elements) {
        std::set<shared<Object>> set;

        for (auto& element : elements) {
            if (element == nullptr)
                throw std::runtime_error("NULLPOINTEREXCEPTION!!!!");
            if (set.contains(element))
                throw std::runtime_error("ILLEGALARGUMENTEXCEPTION!!!!");
            set.insert(element);
        }

        return alloc<Set_Immutable_Specialization>(std::move(set));
    }

    class Spliterator_Set_Specialization final : public virtual Object, public virtual Spliterator {
        shared<Set> set;
        shared<Iterator> iterator;

    public:
        explicit Spliterator_Set_Specialization(const shared<Set> &obj) : set(obj) {
            iterator = set->iterator();
        }

        ~Spliterator_Set_Specialization() override = default;

        int characteristics() override {
            return SIZED;
        }

        long long estimateSize() override {
            return set->size();
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

    shared<Spliterator> Set::spliterator() {
        return alloc<Spliterator_Set_Specialization>(std::dynamic_pointer_cast<Set>(shared_from_this()));
    }
}
