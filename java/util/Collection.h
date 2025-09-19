#pragma once

#include <Array.h>
#include <Object.h>

#include "Iterable.h"

namespace java::util {
    namespace function {
        class IntFunction;
        class Predicate;
    }

    namespace stream {
        class Stream;
    }

    class Collection : public virtual Object, public virtual Iterable {
    public:
        ~Collection() override = default;

        virtual bool add(shared<Object> e) = 0;

        virtual bool addAll(shared<Collection> c) = 0;

        virtual void clear() = 0;

        virtual bool contains(shared<Object> o) = 0;

        virtual bool containsAll(shared<Collection> c) = 0;

        bool equals(Object *obj) override;

        DEFINE_SHARED_EQUALS

        int hashCode() override;

        virtual bool isEmpty() = 0;

        shared<Iterator> iterator() override = 0;

        virtual shared<stream::Stream> parallelStream();

        virtual bool remove(shared<Object> o) = 0;

        virtual bool removeAll(shared<Collection> c) = 0;

        virtual bool removeIf(shared<function::Predicate> filter);

        virtual bool retainAll(shared<Collection> c) = 0;

        virtual int size() = 0;

        shared<Spliterator> spliterator() override;

        virtual shared<stream::Stream> stream();

        virtual shared<Array<>> toArray() = 0;

        virtual shared<Array<>> toArray(shared<function::IntFunction> generator);

        virtual shared<Array<>> toArray(shared<Array<>> a) = 0;
    };
}
