#pragma once

#include <Cloneable.h>
#include <Object.h>

#include "List.h"
#include "AbstractList.h"
#include "RandomAccess.h"

#include <java/io/Serializable.h>

namespace java::util {
    class ArrayList : public virtual Object, public virtual List, public virtual RandomAccess, public virtual Cloneable,
                      public virtual AbstractList, public virtual io::Serializable {
        shared<Object>* _array;
        int _size;
        int _capacity;

        void copyFrom(ArrayList *other);

        void checkBounds(int index) const;

        void resize(int newSize);

        friend class ArrayList_Spliterator_Specialization;

    public:
        ArrayList();

        explicit ArrayList(int initialCapacity);

        explicit ArrayList(const shared<Collection> &c);

        void add(int index, shared<Object> element) override;

        bool add(shared<Object> e) override;

        bool addAll(int index, shared<Collection> c) override;

        bool addAll(shared<Collection> c) override;

        void clear() override;

        std::shared_ptr<Object> clone() override;

        bool contains(shared<Object> o) override;

        virtual void ensureCapacity(int minCapacity);

        bool equals(Object *obj) override;

        void forEach(shared<function::Consumer> action) override;

        shared<Object> get(int index) override;

        int hashCode() override;

        int indexOf(shared<Object> o) override;

        bool isEmpty() override;

        shared<Iterator> iterator() override;

        int lastIndexOf(shared<Object> o) override;

        shared<ListIterator> listIterator() override;

        shared<ListIterator> listIterator(int index) override;

        shared<Object> remove(int index) override;

        bool remove(shared<Object> o) override;

        bool removeAll(shared<Collection> c) override;

        bool removeIf(shared<function::Predicate> filter) override;

    protected:
        void removeRange(int fromIndex, int toIndex) override;

    public:
        bool retainAll(shared<Collection> c) override;

        shared<Object> set(int index, shared<Object> element) override;

        int size() override;

        shared<Spliterator> spliterator() override;

        shared<List> subList(int fromIndex, int toIndex) override;

        shared<Array<>> toArray() override;

        shared<Array<>> toArray(shared<function::IntFunction> generator) override;

        shared<Array<>> toArray(shared<Array<>> a) override;

        virtual void trimToSize();
    };
}
