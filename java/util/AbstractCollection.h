#pragma once

#include <Object.h>
#include "Collection.h"

namespace java::util {
    class AbstractCollection : public virtual Object, public virtual Collection {
    protected:
        AbstractCollection() = default;

    public:
        ~AbstractCollection() override = default;

        bool add(shared<Object> e) override;

        bool addAll(shared<Collection> c) override;

        void clear() override;

        bool contains(shared<Object> o) override;

        bool containsAll(shared<Collection> c) override;

        bool isEmpty() override;

        shared<Iterator> iterator() override = 0;

        bool remove(shared<Object> o) override;

        bool removeAll(shared<Collection> c) override;

        bool retainAll(shared<Collection> c) override;

        shared<Array<>> toArray() override;

        shared<Array<>> toArray(shared<Array<>> a) override;

        using Collection::toArray;

        shared<String> toString() override;
    };

}
