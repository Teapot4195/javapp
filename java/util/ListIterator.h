#pragma once

#include <Object.h>

#include "Iterator.h"

namespace java::util {
    class ListIterator : public virtual Object, public virtual Iterator {
    public:
        ~ListIterator() override = default;

        virtual void add(shared<Object> e) = 0;

        bool hasNext() override = 0;

        virtual bool hasPrevious() = 0;

        shared<Object> next() override = 0;

        virtual int nextIndex() = 0;

        virtual shared<Object> previous() = 0;

        virtual int previousIndex() = 0;

        void remove() override = 0;

        virtual void set(shared<Object> e) = 0;
    };
}
