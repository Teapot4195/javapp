#include "AbstractSet.h"

#include "Iterator.h"

namespace java::util {
    bool AbstractSet::equals(Object *obj) {
        if (this == obj)
            return true;
        if (!obj->instanceof<Set>())
            return false;
        auto* set = dynamic_cast<Set*>(obj);
        if (set->size() != size())
            return false;
        return this->containsAll(std::dynamic_pointer_cast<Collection>(set->shared_from_this()));
    }

    int AbstractSet::hashCode() {
        int hash = 0;

        const auto it = iterator();
        while (it->hasNext()) {
            if (auto elem = it->next(); elem != nullptr)
                hash += elem->hashCode();
        }

        return hash;
    }

    bool AbstractSet::removeAll(const shared<Collection> c) {
        bool changed = false;
        const auto it = iterator();
        while (it->hasNext()) {
            if (c->contains(it->next())) {
                changed = true;
                it->remove();
            }
        }

        return changed;
    }

    bool AbstractSet::containsAll(const shared<Collection> c) {
        return AbstractCollection::containsAll(c);
    }

    shared<Iterator> AbstractSet::iterator() {
        return AbstractCollection::iterator();
    }

    bool AbstractSet::add(const shared<Object> e) {
        return AbstractCollection::add(e);
    }

    bool AbstractSet::addAll(const shared<Collection> c) {
        return AbstractCollection::addAll(c);
    }

    void AbstractSet::clear() {
        AbstractCollection::clear();
    }

    bool AbstractSet::contains(const shared<Object> o) {
        return AbstractCollection::contains(o);
    }

    bool AbstractSet::isEmpty() {
        return AbstractCollection::isEmpty();
    }

    bool AbstractSet::remove(const shared<Object> o) {
        return AbstractCollection::remove(o);
    }

    bool AbstractSet::retainAll(const shared<Collection> c) {
        return AbstractCollection::retainAll(c);
    }

    shared<Array<>> AbstractSet::toArray() {
        return AbstractCollection::toArray();
    }

    shared<Array<>> AbstractSet::toArray(const shared<Array<>> a) {
        return AbstractCollection::toArray(a);
    }

    shared<Array<>> AbstractSet::toArray(const shared<function::IntFunction> generator) {
        return AbstractCollection::toArray(generator);
    }
}
