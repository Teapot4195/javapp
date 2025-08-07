#pragma once

#include "AbstractCollection.h"
#include <Object.h>
#include "Set.h"

namespace java::util {
    class AbstractSet : public virtual Object, public virtual AbstractCollection, public virtual Set {
    protected:
        AbstractSet() = default;

    public:
        bool equals(Object *obj) override;

        DEFINE_SHARED_EQUALS

        int hashCode() override;

        bool removeAll(shared<Collection> c) override;

        bool containsAll(shared<Collection> c) override;

        shared<Iterator> iterator() override;

        bool add(shared<Object> e) override;

        bool addAll(shared<Collection> c) override;

        void clear() override;

        bool contains(shared<Object> o) override;

        bool isEmpty() override;

        bool remove(shared<Object> o) override;

        bool retainAll(shared<Collection> c) override;

        shared<Array<>> toArray() override;

        shared<Array<>> toArray(shared<Array<>> a) override;

        shared<Array<>> toArray(shared<function::IntFunction> generator) override;
    };
}
