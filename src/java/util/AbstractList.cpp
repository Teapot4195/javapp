#include "AbstractList.h"

#include "ListIterator.h"

namespace java::util {
    void AbstractList::add(int index, shared<Object> element) {
        throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION");
    }

    bool AbstractList::add(const shared<Object> e) {
        add(size(), e);
        return true;
    }

    bool AbstractList::addAll(int index, const shared<Collection> c) {
        const auto it = c->iterator();

        while (it->hasNext()) {
            add(index++, it->next());
        }

        return true;
    }

    bool AbstractList::addAll(const shared<Collection> c) {
        return AbstractCollection::addAll(c);
    }

    void AbstractList::clear() {
        removeRange(0, size());
    }

    bool AbstractList::contains(const shared<Object> o) {
        return AbstractCollection::contains(o);
    }

    bool AbstractList::containsAll(const shared<Collection> c) {
        return AbstractCollection::containsAll(c);
    }

    bool AbstractList::equals(Object *obj) {
        if (obj == this)
            return true;
        if (!obj->instanceof<List>())
            return false;
        const auto list = dynamic_cast<List*>(obj);
        const auto ita = this->iterator(), itb = list->iterator();
        while (true) {
            const auto an = ita->hasNext(), bn = itb->hasNext();
            if (an && bn) {
                auto ann = ita->next(), bnn = itb->next();

                if (ann == nullptr && bnn == nullptr)
                    continue;
                if ((ann == nullptr) != (bnn == nullptr))
                    return false;
                if (ann->equals(bnn))
                    continue;
                return false;
            }
            if (an || bn)
                return false;
            return true;
        }
    }

    int AbstractList::hashCode() {
        int hashCode = 1;
        const auto it = iterator();

        while (it->hasNext()) {
            auto e = it->next();
            hashCode = 31 * hashCode + (e == nullptr ? 0 : e->hashCode());
        }

        return hashCode;
    }

    int AbstractList::indexOf(const shared<Object> o) {
        const auto it = listIterator();

        while (it->hasNext()) {
            if (it->next()->equals(o)) {
                return it->previousIndex();
            }
        }

        return -1;
    }

    class AbstractList_Iterator_Specialization : public virtual Object, public virtual Iterator {
    protected:
        int index = 0;
        int expectedModCount = 0;

        shared<AbstractList> parent;

    public:
        explicit AbstractList_Iterator_Specialization(const shared<AbstractList> &parent) : expectedModCount(parent->modCount), parent(parent) {}

        explicit AbstractList_Iterator_Specialization(const shared<AbstractList> &parent, const int begIndex) :
            index(begIndex), expectedModCount(parent->modCount), parent(parent) {}

        bool hasNext() override {
            return index < parent->size();
        }

        shared<Object> next() override {
            if (expectedModCount != parent->modCount)
                throw std::runtime_error("THROW CONCURRENTMODIFICATIONEXCEPTION");
            return parent->get(index++);
        }

        void remove() override {
            parent->remove(index - 1);
        }
    };

    shared<Iterator> AbstractList::iterator() {
        return alloc<AbstractList_Iterator_Specialization>(std::dynamic_pointer_cast<AbstractList>(shared_from_this()));
    }

    int AbstractList::lastIndexOf(const shared<Object> o) {
        const auto it = listIterator(size());

        while (it->hasPrevious()) {
            if (it->previous()->equals(o)) {
                return it->nextIndex();
            }
        }

        return -1;
    }

    shared<ListIterator> AbstractList::listIterator() {
        return listIterator(0);
    }

    class AbstractList_ListIterator_Specialization final : public virtual Object, public virtual ListIterator, public virtual AbstractList_Iterator_Specialization {
    public:
        using AbstractList_Iterator_Specialization::AbstractList_Iterator_Specialization;

        void add(const shared<Object> e) override {
            parent->add(index, e);
        }

        bool hasNext() override {
            return AbstractList_Iterator_Specialization::hasNext();
        }

        bool hasPrevious() override {
            return index > 0;
        }

        shared<Object> next() override {
            return AbstractList_Iterator_Specialization::next();
        }

        int nextIndex() override {
            return index;
        }

        shared<Object> previous() override {
            if (expectedModCount != parent->modCount)
                throw std::runtime_error("THROW CONCURRENTMODIFICATIONEXCEPTION");
            return parent->get(--index);
        }

        int previousIndex() override {
            return index - 1;
        }

        void remove() override {
            parent->remove(index - 1);
            index--;
        }

        void set(const shared<Object> e) override {
            parent->set(index - 1, e);
        }
    };

    shared<ListIterator> AbstractList::listIterator(int index) {
        if (index < 0 || index >= size())
            throw std::runtime_error("THROW CONCURRENTMODIFICATIONEXCEPTION");
        return alloc<AbstractList_ListIterator_Specialization>(std::dynamic_pointer_cast<AbstractList>(shared_from_this()), index);
    }

    shared<Object> AbstractList::remove(int index) {
        throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION");
    }

    bool AbstractList::remove(const shared<Object> o) {
        return AbstractCollection::remove(o);
    }

    bool AbstractList::removeAll(const shared<Collection> c) {
        return AbstractCollection::removeAll(c);
    }

    bool AbstractList::retainAll(const shared<Collection> c) {
        return AbstractCollection::retainAll(c);
    }

    void AbstractList::removeRange(const int fromIndex, const int toIndex) {
        const auto it = listIterator(fromIndex);
        for (int i = 0; i < toIndex - fromIndex; i++) {
            it->next();
            it->remove();
        }
    }

    shared<Object> AbstractList::set(int index, shared<Object> element) {
        throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION");
    }

    class AbstractList_SubList_Specialization final : public virtual Object, public virtual AbstractList {
        int begin;
        int _size;
        int expectedModCount;

        shared<AbstractList> parent;

    public:
        explicit AbstractList_SubList_Specialization(const shared<AbstractList>& parent, const int beg, const int size)
          : begin(beg), _size(size), expectedModCount(parent->modCount), parent(parent) {}

        shared<Object> set(const int index, const shared<Object> element) override {
            if (expectedModCount != parent->modCount)
                throw std::runtime_error("THROW CONCURRENTMODIFICATIONEXCEPTION");
            if (index < 0 || index >= _size)
                throw std::runtime_error("THROW INDEXOUTOFBOUNDSEXCEPTION");
            return parent->set(index + begin, element);
        }

        shared<Object> get(const int index) override {
            if (expectedModCount != parent->modCount)
                throw std::runtime_error("THROW CONCURRENTMODIFICATIONEXCEPTION");
            if (index < 0 || index >= _size)
                throw std::runtime_error("THROW INDEXOUTOFBOUNDSEXCEPTION");
            return parent->get(index + begin);
        }

        void add(const int index, const shared<Object> element) override {
            if (expectedModCount != parent->modCount)
                throw std::runtime_error("THROW CONCURRENTMODIFICATIONEXCEPTION");
            if (index < 0 || index >= _size)
                throw std::runtime_error("THROW INDEXOUTOFBOUNDSEXCEPTION");
            return parent->add(index + begin, element);
        }

        shared<Object> remove(const int index) override {
            if (expectedModCount != parent->modCount)
                throw std::runtime_error("THROW CONCURRENTMODIFICATIONEXCEPTION");
            if (index < 0 || index >= _size)
                throw std::runtime_error("THROW INDEXOUTOFBOUNDSEXCEPTION");
            return parent->remove(index + begin);
        }

        bool addAll(const int index, const shared<Collection> c) override {
            if (expectedModCount != parent->modCount)
                throw std::runtime_error("THROW CONCURRENTMODIFICATIONEXCEPTION");
            if (index < 0 || index >= _size)
                throw std::runtime_error("THROW INDEXOUTOFBOUNDSEXCEPTION");
            return parent->addAll(index + begin, c);
        }

        void removeRange(const int fromIndex, const int toIndex) override {
            if (expectedModCount != parent->modCount)
                throw std::runtime_error("THROW CONCURRENTMODIFICATIONEXCEPTION");
            if (fromIndex < 0 || toIndex >= _size)
                throw std::runtime_error("THROW INDEXOUTOFBOUNDSEXCEPTION");
            if (fromIndex > toIndex)
                throw std::runtime_error("THROW ILLEGALARGUMENTEXCEPTION");
            parent->removeRange(fromIndex, toIndex);
        }

        bool addAll(const shared<Collection> c) override {
            return addAll(begin + _size, c);
        }

        void forEach(const shared<function::Consumer> action) override {
            return parent->forEach(action);
        }

        shared<stream::Stream> parallelStream() override {
            return parent->parallelStream();
        }

        bool removeIf(const shared<function::Predicate> filter) override {
            return parent->removeIf(filter);
        }

        shared<stream::Stream> stream() override {
            return parent->stream();
        }

        shared<Array<>> toArray(const shared<function::IntFunction> generator) override {
            return parent->toArray(generator);
        }

        bool contains(const shared<Object> o) override {
            return parent->contains(o);
        }

        bool containsAll(const shared<Collection> c) override {
            return parent->containsAll(c);
        }

        bool isEmpty() override {
            return parent->isEmpty();
        }

        bool remove(shared<Object> o) override {
            return parent->isEmpty();
        }

        bool removeAll(const shared<Collection> c) override {
            return parent->removeAll(c);
        }

        bool retainAll(const shared<Collection> c) override {
            return parent->retainAll(c);
        }

        shared<Array<>> toArray() override {
            return parent->toArray();
        }

        shared<Array<>> toArray(const shared<Array<>> a) override {
            return parent->toArray(a);
        }

        shared<String> toString() override {
            return parent->toString();
        }

        void replaceAll(const shared<function::UnaryOperator> op) override {
            return parent->replaceAll(op);
        }

        int size() override {
            return _size;
        }

        void sort(shared<Comparator> c) override {
            // TODO: sort the child, this will cause less issues, although it is not specified how the child should behave in this case.
            throw std::runtime_error("ILLEGALOPERATIONEXCEPTION, sort on a sublist will surely cause many issues.");
        }

        shared<Spliterator> spliterator() override {
            return parent->spliterator();
        }

        ~AbstractList_SubList_Specialization() override = default;

        bool add(const shared<Object> e) override {
            return parent->add(e);
        }

        void clear() override {
            return parent->clear();
        }

        bool equals(Object *obj) override {
            return parent->equals(obj);
        }

        int hashCode() override {
            return parent->hashCode();
        }

        int indexOf(const shared<Object> o) override {
            return parent->indexOf(o);
        }

        shared<Iterator> iterator() override {
            return parent->iterator();
        }

        int lastIndexOf(const shared<Object> o) override {
            return parent->lastIndexOf(o);
        }

        shared<ListIterator> listIterator() override {
            return listIterator(0);
        }

        shared<ListIterator> listIterator(const int index) override {
            return alloc<AbstractList_ListIterator_Specialization>(std::dynamic_pointer_cast<AbstractList>(shared_from_this()), 0);
        }

        shared<List> subList(const int fromIndex, const int toIndex) override {
            return parent->subList(fromIndex + begin, toIndex + begin);
        }
    };

    shared<List> AbstractList::subList(int fromIndex, const int toIndex) {
        return alloc<AbstractList_SubList_Specialization>(std::dynamic_pointer_cast<AbstractList>(shared_from_this()), fromIndex, toIndex - fromIndex);
    }

    shared<Array<>> AbstractList::toArray() {
        return AbstractCollection::toArray();
    }

    shared<Array<>> AbstractList::toArray(const shared<function::IntFunction> generator) {
        return AbstractCollection::toArray(generator);
    }

    shared<Array<>> AbstractList::toArray(const shared<Array<>> a) {
        return AbstractCollection::toArray(a);
    }
}
