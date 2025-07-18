#include "List.h"

#include "ListIterator.h"
#include "RandomAccess.h"
#include "Spliterator.h"
#include "function/Consumer.h"
#include "function/UnaryOperator.h"

namespace java::util {
    class List_CopyOf_Immutable_Iterator : public virtual Object, public virtual Iterator {
        shared<List> parent;
        int index = 0;

    public:
        ~List_CopyOf_Immutable_Iterator() override = default;

        explicit List_CopyOf_Immutable_Iterator(const shared<List> &parent) : parent(parent) {}

        bool hasNext() override {
            return index + 1 != parent->size();
        }

        shared<Object> next() override {
            return parent->get(index++);
        }
    };

    class List_CopyOf_Immutable_ListIterator final : public virtual Object, public virtual ListIterator, public virtual List_CopyOf_Immutable_Iterator {
        shared<List> parent;
        int index = 0;

    public:
        ~List_CopyOf_Immutable_ListIterator() override = default;

        using List_CopyOf_Immutable_Iterator::List_CopyOf_Immutable_Iterator;

        explicit List_CopyOf_Immutable_ListIterator(const shared<List> &parent, int index) : List_CopyOf_Immutable_Iterator(parent), index(index) {}

        bool hasNext() override {
            return index + 1 != parent->size();
        }

        shared<Object> next() override {
            return parent->get(index++);
        }

        void add(shared<Object> e) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool hasPrevious() override {
            return index != 0;
        }

        int nextIndex() override {
            return index + 1;
        }

        shared<Object> previous() override {
            return parent->get(index--);
        }

        int previousIndex() override {
            return index == 0 ? 0 : index - 1;
        }

        void remove() override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        void set(shared<Object> e) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }
    };

    class List_CopyOf_Immutable;

    class List_CopyOf_Immutable_SubList final : public virtual Object, public virtual List {
        shared<List_CopyOf_Immutable> parent;
        int begin, end;

    public:
        explicit List_CopyOf_Immutable_SubList(const shared<List_CopyOf_Immutable> &parent, int begin, int end) : parent(parent), begin(begin), end(end) {}

        void add(int index, shared<Object> element) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool add(shared<Object> e) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool addAll(int index, shared<Collection> c) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool addAll(shared<Collection> c) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        void clear() override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool contains(shared<Object> o) override;

        bool containsAll(shared<Collection> c) override;

        shared<Object> get(int index) override;

        int indexOf(shared<Object> o) override;

        bool isEmpty() override;

        shared<Iterator> iterator() override {
            return alloc<List_CopyOf_Immutable_Iterator>(std::dynamic_pointer_cast<List>(shared_from_this()));
        }

        int lastIndexOf(shared<Object> o) override;

        shared<ListIterator> listIterator() override {
            return alloc<List_CopyOf_Immutable_ListIterator>(std::dynamic_pointer_cast<List>(shared_from_this()));
        }

        shared<ListIterator> listIterator(int index) override {
            return alloc<List_CopyOf_Immutable_ListIterator>(std::dynamic_pointer_cast<List>(shared_from_this()), index);
        }

        shared<Object> remove(int index) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool remove(shared<Object> o) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool removeAll(shared<Collection> c) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool retainAll(shared<Collection> c) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        shared<Object> set(int index, shared<Object> element) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        int size() override;

        shared<List> subList(int fromIndex, int toIndex) override;

        shared<Array<>> toArray() override {
            return alloc<Array<>>(parent);
        }

        shared<Array<>> toArray(shared<Array<>> a) override;
    };

    class List_CopyOf_Immutable final : public virtual Object, public virtual List {
        shared<Array<>> parent;

        friend class List_CopyOf_Immutable_SubList;

    public:
        explicit List_CopyOf_Immutable(const shared<Collection> &parent) : parent(parent->toArray()) {}

        explicit List_CopyOf_Immutable(const shared<Array<>> &parent) : parent(parent) {}

        void add(int index, shared<Object> element) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool add(shared<Object> e) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool addAll(int index, shared<Collection> c) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool addAll(shared<Collection> c) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        void clear() override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool contains(const shared<Object> o) override {
            const auto& d = parent->data;
            for (int i = 0; i < parent->length; i++) {
                if (d[i] != nullptr && d[i]->equals(o))
                    return true;
                if (d[i] == nullptr && o == nullptr)
                    return true;
            }
            return false;
        }

        bool containsAll(const shared<Collection> c) override {
            const auto it = c->iterator();
            while (it->hasNext()) {
                if (const auto next = it->next(); !contains(next))
                    return false;
            }
            return true;
        }

        shared<Object> get(const int index) override {
            return parent->data[index];
        }

        int indexOf(const shared<Object> o) override {
            const auto& d = parent->data;
            for (int i = 0; i < parent->length; i++) {
                if (d[i] != nullptr && d[i]->equals(o))
                    return i;
                if (d[i] == nullptr && o == nullptr)
                    return i;
            }
            return -1;
        }

        bool isEmpty() override {
            return parent->length == 0;
        }

        shared<Iterator> iterator() override {
            return alloc<List_CopyOf_Immutable_Iterator>(std::dynamic_pointer_cast<List>(shared_from_this()));
        }

        int lastIndexOf(const shared<Object> o) override {
            const auto& d = parent->data;
            for (int i = parent->length - 1; i >= 0; i--) {
                if (d[i] != nullptr && d[i]->equals(o))
                    return i;
                if (d[i] == nullptr && o == nullptr)
                    return i;
            }
            return -1;
        }

        shared<ListIterator> listIterator() override {
            return alloc<List_CopyOf_Immutable_ListIterator>(std::dynamic_pointer_cast<List>(shared_from_this()));
        }

        shared<ListIterator> listIterator(int index) override {
            return alloc<List_CopyOf_Immutable_ListIterator>(std::dynamic_pointer_cast<List>(shared_from_this()), index);
        }

        shared<Object> remove(int index) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool remove(shared<Object> o) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool removeAll(shared<Collection> c) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        bool retainAll(shared<Collection> c) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        shared<Object> set(int index, shared<Object> element) override {
            throw std::runtime_error("THROW UNSUPPORTEDEXCEPTION!");
        }

        int size() override {
            return parent->length;
        }

        shared<List> subList(int fromIndex, int toIndex) override {
            return alloc<List_CopyOf_Immutable_SubList>(std::dynamic_pointer_cast<List_CopyOf_Immutable>(shared_from_this()), fromIndex, toIndex);
        }

        shared<Array<>> toArray() override {
            return alloc<Array<>>(parent);
        }

        shared<Array<>> toArray(shared<Array<>> a) override {
            if (parent->length < a->length) {
                for (int i = 0; i < parent->length; i++) {
                    a->data[i] = parent->data[i];
                }
                return a;
            }
            a->resize(parent->length);
            for (int i = 0; i < parent->length; i++) {
                a->data[i] = parent->data[i];
            }
            return a;
        }
    };

    bool List_CopyOf_Immutable_SubList::contains(const shared<Object> o) {
        const auto& d = parent->parent->data;
        for (int i = begin; i < end; i++) {
            if (d[i] != nullptr && d[i]->equals(o))
                return true;
            if (d[i] == nullptr && o == nullptr)
                return true;
        }
        return false;
    }

    bool List_CopyOf_Immutable_SubList::containsAll(const shared<Collection> c) {
        const auto it = c->iterator();
        while (it->hasNext()) {
            if (const auto next = it->next(); !contains(next))
                return false;
        }
        return true;
    }

    shared<Object> List_CopyOf_Immutable_SubList::get(const int index) {
        return parent->parent->data[begin + index];
    }

    int List_CopyOf_Immutable_SubList::indexOf(const shared<Object> o) {
        const auto& d = parent->parent->data;
        for (int i = begin; i < end; i++) {
            if (d[i] != nullptr && d[i]->equals(o))
                return i;
            if (d[i] == nullptr && o == nullptr)
                return i;
        }
        return -1;
    }

    bool List_CopyOf_Immutable_SubList::isEmpty() {
        return end - begin == 0;
    }

    int List_CopyOf_Immutable_SubList::lastIndexOf(const shared<Object> o) {
        const auto& d = parent->parent->data;
        for (int i = end - 1; i >= begin; i--) {
            if (d[i] != nullptr && d[i]->equals(o))
                return i;
            if (d[i] == nullptr && o == nullptr)
                return i;
        }
        return -1;
    }

    int List_CopyOf_Immutable_SubList::size() {
        return end - begin;
    }

    shared<List> List_CopyOf_Immutable_SubList::subList(int fromIndex, int toIndex) {
        return alloc<List_CopyOf_Immutable_SubList>(this, fromIndex, toIndex);
    }

    shared<Array<>> List_CopyOf_Immutable_SubList::toArray(shared<Array<>> a) {
        if (size() < a->length) {
            for (int i = begin; i < end; i++) {
                a->data[i] = parent->parent->data[i];
            }
            return a;
        }
        a->resize(size());
        for (int i = begin; i < end; i++) {
            a->data[i] = parent->parent->data[i];
        }
        return a;
    }

    shared<List> List::copyOf(const shared<Collection>& coll) {
        return alloc<List_CopyOf_Immutable>(coll);
    }

    shared<List> List::of() {
        return alloc<List_CopyOf_Immutable>(alloc<Array<>>());
    }

    shared<List> List::of(const std::vector<shared<Object>>& elements) {
        return alloc<List_CopyOf_Immutable>(elements);
    }

    void List::replaceAll(const shared<function::UnaryOperator> op) {
        shared<ListIterator> li = this->listIterator();
        while (li->hasNext())
            li->set(op->apply(li->next()));
    }

    void List::sort(shared<Comparator> c) {
#warning default sort is not valid!
        throw std::runtime_error("sort does not work!");
    }

    class List_Spliterator_NonRandom final : public virtual Object, public virtual Spliterator {
        shared<List> parent;
        shared<Iterator> it;

    public:
        ~List_Spliterator_NonRandom() override = default;

        explicit List_Spliterator_NonRandom(const shared<List> &parent) : parent(parent), it(parent->iterator()) {}

        int characteristics() override {
            return SIZED;
        }

        long long estimateSize() override {
            return parent->size();
        }

        bool tryAdvance(const shared<function::Consumer> action) override {
            if (!it->hasNext())
                return false;
            action->accept(it->next());
            return true;
        }

        shared<Spliterator> trySplit() override {
            return nullptr;
        }
    };

    class List_Spliterator_Random final : public virtual Object, public virtual Spliterator {
        shared<List> parent;
        int index;

    public:
        ~List_Spliterator_Random() override = default;

        explicit List_Spliterator_Random(const shared<List> &parent) : parent(parent), index(0) {}

        int characteristics() override {
            return SIZED;
        }

        long long estimateSize() override {
            return parent->size();
        }

        bool tryAdvance(const shared<function::Consumer> action) override {
#warning NON COMPLIANCE fail-fast behavior missing
            action->accept(parent->get(index++));
            return true;
        }

        shared<Spliterator> trySplit() override {
            return nullptr;
        }
    };

    shared<Spliterator> List::spliterator() {
        if (shared_from_this()->instanceof<RandomAccess>())
            return alloc<List_Spliterator_Random>(std::dynamic_pointer_cast<List>(shared_from_this()));
        return alloc<List_Spliterator_NonRandom>(std::dynamic_pointer_cast<List>(shared_from_this()));
    }
}
