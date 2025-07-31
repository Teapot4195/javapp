#include "AbstractCollection.h"

#include "Iterator.h"
#include "String.h"

namespace java::util {
    bool AbstractCollection::add(shared<Object> e) {
        throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION");
    }

    bool AbstractCollection::addAll(shared<Collection> c) {
        const auto it = c->iterator();

        while (it->hasNext()) {
            add(it->next());
        }

        return true;
    }

    void AbstractCollection::clear() {
        const auto it = iterator();

        while (it->hasNext()) {
            it->remove();
        }
    }

    bool AbstractCollection::contains(shared<Object> o) {
        const auto it = iterator();

        while (it->hasNext()) {
            if (it->next()->equals(o))
                return true;
        }

        return false;
    }

    bool AbstractCollection::containsAll(shared<Collection> c) {
        const auto it = c->iterator();

        while (it->hasNext()) {
            if (!contains(it->next()))
                return false;
        }

        return true;
    }

    bool AbstractCollection::isEmpty() {
        return size() == 0;
    }

    bool AbstractCollection::remove(shared<Object> o) {
        const auto it = iterator();

        while (it->hasNext()) {
            if (it->next()->equals(o)) {
                it->remove();
                return true;
            }
        }

        return false;
    }

    bool AbstractCollection::removeAll(shared<Collection> c) {
        const auto it = c->iterator();

        bool changed = false;

        while (it->hasNext()) {
            remove(it->next());
            changed = true;
        }

        return changed;
    }

    bool AbstractCollection::retainAll(shared<Collection> c) {
        const auto it = iterator();

        bool changed = false;

        while (it->hasNext()) {
            if (!c->contains(it->next())) {
                it->remove();
                changed = true;
            }
        }

        return changed;
    }

    shared<Array<>> AbstractCollection::toArray() {
        auto array = alloc<Array<>>(size());

        return toArray(array);
    }

    // TODO: document specifies that
    shared<Array<>> AbstractCollection::toArray(shared<Array<>> a) {
        auto s = size();
        auto it = iterator();

        if (a == nullptr)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION");

        if (a->length < s) {
            a->resize(s);
        }

        int index = 0;

        while (it->hasNext()) {
            a->data[index++] = it->next();
        }

        if (index < a->length) {
            for (int i = index; i < a->length; i++) {
                a->data[i] = nullptr;
            }
        }

        return a;
    }

    shared<String> AbstractCollection::toString() {
        std::string result = "[";
        auto it = iterator();

#warning needs to use String.valueOf(Object) not object->toString() (for some reason)
        while (it->hasNext()) {
            result += it->next()->toString()->get_backing();
            if (it->hasNext()) {
                result += ", ";
            }
        }

        return alloc<String>(result + "]");
    }
}
