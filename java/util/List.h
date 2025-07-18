#pragma once

#include "Collection.h"
#include <Object.h>

namespace java::util::function {
    class UnaryOperator;
}

namespace java::util {
    class Comparator;
    class ListIterator;
}

namespace java::util {
    class List : public virtual Object, public virtual Collection {
    public:
        ~List() override = default;

        virtual void add(int index, shared<Object> element) = 0;

        bool add(shared<Object> e) override = 0;

        virtual bool addAll(int index, shared<Collection> c) = 0;

        bool addAll(shared<Collection> c) override = 0;

        void clear() override = 0;

        bool contains(shared<Object> o) override = 0;

        bool containsAll(shared<Collection> c) override = 0;

        static shared<List> copyOf(const shared<Collection>& coll);

        virtual shared<Object> get(int index) = 0;

        virtual int indexOf(shared<Object> o) = 0;

        bool isEmpty() override = 0;

        shared<Iterator> iterator() override = 0;

        virtual int lastIndexOf(shared<Object> o) = 0;

        virtual shared<ListIterator> listIterator() = 0;

        virtual shared<ListIterator> listIterator(int index) = 0;

        static shared<List> of();

        static shared<List> of(const std::vector<shared<Object>>& elements);

        template <typename... Args>
        static shared<List> of(Args&&... args) {
            return of({std::forward<Args>(args)...});
        }

        virtual shared<Object> remove(int index) = 0;

        bool remove(shared<Object> o) override = 0;

        bool removeAll(shared<Collection> c) override = 0;

        virtual void replaceAll(shared<function::UnaryOperator> op);

        bool retainAll(shared<Collection> c) override = 0;

        virtual shared<Object> set(int index, shared<Object> element) = 0;

        int size() override = 0;

        virtual void sort(shared<Comparator> c);

        shared<Spliterator> spliterator() override;

        virtual shared<List> subList(int fromIndex, int toIndex) = 0;

        shared<Array<>> toArray() override = 0;

        shared<Array<>> toArray(shared<Array<>> a) override = 0;
    };
}
