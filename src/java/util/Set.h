#pragma once

#include <Object.h>

#include "Collection.h"

namespace java::util {
    class Set : public virtual Object, public virtual Collection {
    public:
        ~Set() override = default;

        bool add(shared<Object> e) override = 0;

        bool addAll(shared<Collection> c) override = 0;

        void clear() override = 0;

        bool contains(shared<Object> o) override = 0;

        bool containsAll(shared<Collection> c) override = 0;

        static shared<Set> copyOf(const shared<Collection>& coll);

        bool isEmpty() override = 0;

        shared<Iterator> iterator() override = 0;

        static shared<Set> of(const std::vector<shared<Object>>& elements);

        template <typename... Args>
        static shared<Set> of(Args&&... args) {
            return of({std::forward<Args>(args)...});
        }

        bool remove(shared<Object> o) override = 0;

        bool removeAll(shared<Collection> c) override = 0;

        bool retainAll(shared<Collection> c) override = 0;

        int size() override = 0;

        shared<Spliterator> spliterator() override;

        shared<Array<>> toArray() override = 0;

        shared<Array<>> toArray(shared<Array<>> a) override = 0;
    };
}
