#pragma once

#include "AbstractSequentialList.h"
#include "Cloneable.h"
#include "Deque.h"
#include "List.h"
#include <Object.h>
#include <java/io/Serializable.h>

namespace java::util {
    class LinkedList : public virtual Object, public virtual AbstractSequentialList, public virtual List,
            public virtual Deque, public virtual Cloneable, public virtual io::Serializable {
        constexpr void checkBounds(int index) const;

        constexpr void checkNotEmpty() const;

        struct linked_list_node {
            linked_list_node* previous;
            shared<Object> data;
            linked_list_node* next;
        };

        linked_list_node *_head, *_tail;
        int _size;

        friend class LinkedList_ListIterator;
        friend class LinkedList_ListIteratorDescending;

    public:
        LinkedList();

        explicit LinkedList(const shared<Collection> &c);

        void add(int index, shared<Object> element) override;

        bool add(shared<Object> e) override;

        bool addAll(int index, shared<Collection> c) override;

        bool addAll(shared<Collection> c) override;

        void addFirst(shared<Object> e) override;

        void addLast(shared<Object> e) override;

        void clear() override;

        std::shared_ptr<Object> clone() override;

        bool contains(shared<Object> o) override;

        shared<Iterator> descendingIterator() override;

        shared<Object> element() override;

        shared<Object> get(int index) override;

        shared<Object> getFirst() override;

        shared<Object> getLast() override;

        int indexOf(shared<Object> o) override;

        int lastIndexOf(shared<Object> o) override;

        shared<ListIterator> listIterator(int index) override;

        shared<ListIterator> listIterator() override;

        bool offer(shared<Object> e) override;

        bool offerFirst(shared<Object> e) override;

        bool offerLast(shared<Object> e) override;

        shared<Object> peek() override;

        shared<Object> peekFirst() override;

        shared<Object> peekLast() override;

        shared<Object> poll() override;

        shared<Object> pollFirst() override;

        shared<Object> pollLast() override;

        shared<Object> pop() override;

        void push(shared<Object> e) override;

        shared<Object> remove() override;

        shared<Object> remove(int index) override;

        bool remove(shared<Object> o) override;

        shared<Object> removeFirst() override;

        bool removeFirstOccurrence(shared<Object> o) override;

        shared<Object> removeLast() override;

        bool removeLastOccurrence(shared<Object> o) override;

        shared<Object> set(int index, shared<Object> element) override;

        int size() override;

        shared<Spliterator> spliterator() override;

        shared<Array<>> toArray() override;

        shared<Array<>> toArray(shared<function::IntFunction> generator) override;

        shared<Array<>> toArray(shared<Array<>> a) override;
    };

}
