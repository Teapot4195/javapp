#include "AbstractSequentialList.h"

#include "ListIterator.h"

namespace java::util {
    void AbstractSequentialList::checkBounds(const int index) {
        if (index < 0 || index >= size())
            throw std::runtime_error("THROW INDEXOUTOFBOUNDSEXCEPTION!");
    }

    void AbstractSequentialList::add(const int index, const shared<Object> element) {
        checkBounds(index);

        listIterator(index)->add(element);
    }

    bool AbstractSequentialList::add(const shared<Object> e) {
        return AbstractList::add(e);
    }

    bool AbstractSequentialList::addAll(const shared<Collection> c) {
        return AbstractList::addAll(c);
    }

    bool AbstractSequentialList::addAll(const int index, const shared<Collection> c) {
        checkBounds(index);

        const auto it = listIterator(index);
        const auto cit = c->iterator();

        bool changed = false;
        while (cit->hasNext()) {
            it->add(cit->next());
            it->next();
            changed = true;
        }

        return changed;
    }

    shared<Object> AbstractSequentialList::get(const int index) {
        checkBounds(index);

        return listIterator(index)->next();
    }

    shared<Iterator> AbstractSequentialList::iterator() {
        return listIterator(0);
    }

    shared<ListIterator> AbstractSequentialList::listIterator() {
        return AbstractList::listIterator();
    }

    bool AbstractSequentialList::remove(const shared<Object> o) {
        return AbstractList::remove(o);
    }

    shared<Object> AbstractSequentialList::remove(const int index) {
        checkBounds(index);

        const auto it = listIterator(index);
        auto last = it->next();
        it->remove();

        return last;
    }

    shared<Object> AbstractSequentialList::set(const int index, const shared<Object> element) {
        checkBounds(index);

        const auto it = listIterator(index);
        auto last = it->next();
        it->set(element);

        return last;
    }
}
