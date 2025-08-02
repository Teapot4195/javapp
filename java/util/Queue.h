#pragma once

#include "Collection.h"
#include <Object.h>

namespace java::util {
    class Queue : public virtual Object, public virtual Collection {
    public:
        bool add(shared<Object> e) override = 0;

        virtual shared<Object> element() = 0;

        virtual bool offer(shared<Object> e) = 0;

        virtual shared<Object> peek() = 0;

        virtual shared<Object> poll() = 0;

        virtual shared<Object> remove() = 0;
    };

}
