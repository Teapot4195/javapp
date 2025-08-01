#pragma once

#include "AbstractCollection.h"
#include "List.h"
#include <Object.h>

namespace java::util {
    class AbstractList : public virtual Object, public virtual AbstractCollection, public virtual List {
    protected:
        int modCount;

        AbstractList() = default;

        friend class AbstractList_Iterator_Specialization;
        friend class AbstractList_ListIterator_Specialization;
        friend class AbstractList_SubList_Specialization;

    public:
        ~AbstractList() override = default;

        void add(int index, shared<Object> element) override;

        bool add(shared<Object> e) override;

        bool addAll(int index, shared<Collection> c) override;

        bool addAll(shared<Collection> c) override;

        void clear() override;

        bool contains(shared<Object> o) override;

        bool containsAll(shared<Collection> c) override;

        bool equals(Object *obj) override;

        shared<Object> get(int index) override = 0;

        int hashCode() override;

        int indexOf(shared<Object> o) override;

        bool isEmpty() override {
            return AbstractCollection::isEmpty();
        }

        shared<Iterator> iterator() override;

        int lastIndexOf(shared<Object> o) override;

        shared<ListIterator> listIterator() override;

        shared<ListIterator> listIterator(int index) override;

        shared<Object> remove(int index) override;

        bool remove(shared<Object> o) override;

        bool removeAll(shared<Collection> c) override;

        bool retainAll(shared<Collection> c) override;

    protected:
        virtual void removeRange(int fromIndex, int toIndex);

    public:
        shared<Object> set(int index, shared<Object> element) override;

        shared<List> subList(int fromIndex, int toIndex) override;

        shared<Array<>> toArray() override;

        shared<Array<>> toArray(shared<function::IntFunction> generator) override;

        shared<Array<>> toArray(shared<Array<>> a) override;
    };
}
