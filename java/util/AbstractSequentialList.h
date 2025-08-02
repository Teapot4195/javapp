#pragma once

#include "AbstractList.h"
#include <Object.h>

namespace java::util {
    class AbstractSequentialList : public virtual Object, public virtual AbstractList {
    protected:
        AbstractSequentialList() = default;

        void checkBounds(int index);

    public:
        void add(int index, shared<Object> element) override;

        bool add(shared<Object> e) override;

        bool addAll(shared<Collection> c) override;

        bool addAll(int index, shared<Collection> c) override;

        shared<Object> get(int index) override;

        shared<Iterator> iterator() override;

        shared<ListIterator> listIterator() override;

        shared<ListIterator> listIterator(int index) override = 0;

        bool remove(shared<Object> o) override;

        shared<Object> remove(int index) override;

        shared<Object> set(int index, shared<Object> element) override;
    };
}
