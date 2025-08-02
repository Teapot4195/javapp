#pragma once

#include <Object.h>
#include "Queue.h"

namespace java::util {
    class Deque : public virtual Object, public virtual Queue {
    public:
        bool add(shared<Object> e) override = 0;

        bool addAll(shared<Collection> c) override = 0;

        virtual void addFirst(shared<Object> e) = 0;

        virtual void addLast(shared<Object> e) = 0;

        bool contains(shared<Object> e) override = 0;

        virtual shared<Iterator> descendingIterator() = 0;

        shared<Object> element() override = 0;

        virtual shared<Object> getFirst() = 0;

        virtual shared<Object> getLast() = 0;

        shared<Iterator> iterator() override = 0;

        bool offer(shared<Object> e) override = 0;

        virtual bool offerFirst(shared<Object> e) = 0;

        virtual bool offerLast(shared<Object> e) = 0;

        shared<Object> peek() override = 0;

        virtual shared<Object> peekFirst() = 0;

        virtual shared<Object> peekLast() = 0;

        shared<Object> poll() override = 0;

        virtual shared<Object> pollFirst() = 0;

        virtual shared<Object> pollLast() = 0;

        virtual shared<Object> pop() = 0;

        virtual void push(shared<Object> e) = 0;

        shared<Object> remove() override = 0;

        bool remove(shared<Object> o) override = 0;

        virtual shared<Object> removeFirst() = 0;

        virtual bool removeFirstOccurrence(shared<Object> o) = 0;

        virtual shared<Object> removeLast() = 0;

        virtual bool removeLastOccurrence(shared<Object> o) = 0;

        int size() override = 0;
    };
}
