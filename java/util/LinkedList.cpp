#include "LinkedList.h"

// TODO: this is massively subpar and highly unoptimized, especially iterating, iterating O(lim -> (_size^2)/2)

#include "Iterator.h"
#include "ListIterator.h"
#include "Spliterator.h"
#include "function/Consumer.h"

namespace java::util {
    constexpr void LinkedList::checkBounds(const int index) const {
        if (index < 0 || index >= _size)
            throw std::runtime_error("THROW INDEXOUTOFBOUNDSEXCEPTION");
    }

    constexpr void LinkedList::checkNotEmpty() const {
        if (_size == 0)
            throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION");
    }

    LinkedList::LinkedList(): _head(nullptr), _tail(nullptr), _size(0) {}

    LinkedList::LinkedList(const shared<Collection> &c): _head(nullptr), _tail(nullptr), _size(0) {
        const auto it = c->iterator();

        if (it->hasNext()) {
            const auto e = it->next();
            auto *new_tail = new linked_list_node{nullptr, e, nullptr};
            _head = _tail = new_tail;
            _size++;
        }

        while (it->hasNext()) {
            const auto e = it->next();

            auto* new_tail = new linked_list_node{_tail, e, nullptr};
            // SUPPRESS: _tail is going to be non-null, it was already set previously
            _tail->next = new_tail;
            _tail = new_tail;
            _size++;
        }
    }

    void LinkedList::add(const int index, const shared<Object> element) {
        if (index < 0 || index > _size)
            throw std::runtime_error("THROW INDEXOUTOFBOUNDSEXCEPTION");

        auto* new_block = new linked_list_node{nullptr, element, nullptr};

        if (index == 0) {
            // special case we are adding to the front
            _head->previous = new_block;
            new_block->next = _head;
            _head = new_block;

            _size++;

            return;
        }
        if (index == _size) {
            // special case we are adding to the end
            _tail->next = new_block;
            new_block->previous = _tail;
            _tail = new_block;

            _size++;

            return;
        }

        auto* current = _head;
        for (int i = 0; i < index; i++)
            current = current->next;

        current->previous->next = new_block;
        new_block->previous = current->previous;
        current->previous = new_block;
        new_block->next = current;

        _size++;
    }

    bool LinkedList::add(const shared<Object> e) {
        addLast(e);
        return true;
    }

    bool LinkedList::addAll(const int index, const shared<Collection> c) {
        const auto it = c->iterator();

        if (index < 0 || index > _size)
            throw std::runtime_error("THROW INDEXOUTOFBOUNDSEXCEPTION");

        // empty collection
        if (!it->hasNext())
            return false;

        /* The implementation here simply splices a new list segment where it needs to go.*/
        int count = 0;
        auto* nchain_start = new linked_list_node{nullptr, it->next(), nullptr};
        auto* nchain_end = nchain_start;

        while (it->hasNext()) {
            auto* new_block = new linked_list_node{nchain_end, it->next(), nullptr};
            nchain_end->next = new_block;
            nchain_end = new_block;
            count++;
        }

        if (index == 0) {
            // special case we are adding to the front
            _head->previous = nchain_end;
            nchain_end->next = _head;
            _head = nchain_start;

            _size += count;

            return true;
        }

        if (index == _size) {
            // special case where we are adding to the end
            _tail->next = nchain_start;
            nchain_start->previous = _tail;
            _tail = nchain_end;

            _size += count;

            return true;
        }

        // Perform the actual splice
        auto* current = _head;
        for (int i = 0; i < index; i++)
            current = current->next;

        auto* prev = current->previous;

        prev->next = nchain_start;
        nchain_start->previous = prev;
        current->previous = nchain_end;
        nchain_end->next = current;

        _size += count;

        return true;
    }

    bool LinkedList::addAll(const shared<Collection> c) {
        const auto it = c->iterator();

        bool changed = false;
        while (it->hasNext()) {
            changed = true;
            addLast(it->next());
        }

        return changed;
    }

    void LinkedList::addFirst(const shared<Object> e) {
        if (_size == 0) {
            auto* new_head = new linked_list_node{nullptr, e, nullptr};
            _head = _tail = new_head;
            _size++;
            return;
        }

        const auto new_head = new linked_list_node{nullptr, e, _head};
        _head->previous = new_head;
        _head = new_head;
        _size++;
    }

    void LinkedList::addLast(const shared<Object> e) {
        if (_size == 0) {
            auto* new_tail = new linked_list_node{nullptr, e, nullptr};
            _head = _tail = new_tail;
            _size++;
            return;
        }

        const auto new_tail = new linked_list_node{_tail, e, nullptr};
        _tail->next = new_tail;
        _tail = new_tail;
        _size++;
    }

    void LinkedList::clear() {
        const auto* current = _head;

        for (int i = 0; i < _size; i++) {
            const auto* c = current;
            current = current->next;
            delete c;
        }
    }

    std::shared_ptr<Object> LinkedList::clone() {
        auto copy = shared<LinkedList>();

        const auto* current = _head;
        for (int i = 0; i < _size; i++) {
            copy->add(_head->data);
            current = current->next;
        }

        return copy;
    }

    bool LinkedList::contains(const shared<Object> o) {
        const auto* current = _head;

        while (current != nullptr) {
            if (current->data == nullptr) {
                if (o == nullptr)
                    return true;
                continue;
            }
            if (current->data->equals(o))
                return true;

            current = current->next;
        }

        return false;
    }

    class LinkedList_ListIteratorDescending final : public virtual Object, public virtual ListIterator {
        shared<LinkedList> parent;
        int index; ///< represents the index @function next would return
        int modCount;

    public:
        LinkedList_ListIteratorDescending(const shared<LinkedList> &parent, const int index) : parent(parent), index(index),
            modCount(parent->modCount) {}

        void add(const shared<Object> e) override {
            parent->add(index, e);
        }

        bool hasPrevious() override {
            return index != parent->_size;
        }

        bool hasNext() override {
            return index - 1 >= 0;
        }

        shared<Object> previous() override {
            if (index < parent->_size)
                return parent->get(index++);
            throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION");
        }

        int previousIndex() override {
            return index;
        }

        shared<Object> next() override {
            if (index >= 0)
                return parent->get(index--);
            throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION");
        }

        int nextIndex() override {
            return index - 1;
        }

        void remove() override {
            parent->remove(index);
        }

        void set(const shared<Object> e) override {
            parent->set(index, e);
        }
    };

    shared<Iterator> LinkedList::descendingIterator() {
        return std::dynamic_pointer_cast<Iterator>(alloc<LinkedList_ListIteratorDescending>(std::dynamic_pointer_cast<LinkedList>(shared_from_this()), _size));
    }

    shared<Object> LinkedList::element() {
        checkNotEmpty();

        return _head->data;
    }

    shared<Object> LinkedList::get(const int index) {
        checkBounds(index);

        auto* current = _head;
        for (int i = 0; i < index; i++)
            current = current->next;

        return current->data;
    }

    shared<Object> LinkedList::getFirst() {
        checkNotEmpty();

        return _head->data;
    }

    shared<Object> LinkedList::getLast() {
        checkNotEmpty();

        return _tail->data;
    }

    int LinkedList::indexOf(const shared<Object> o) {
        const auto* current = _head;
        int index = 0;

        while (current != nullptr) {
            if (current->data == nullptr) {
                if (o == nullptr)
                    return index;
                continue;
            }
            if (current->data->equals(o))
                return index;

            current = current->next;
            index++;
        }

        return -1;
    }

    shared<Iterator> LinkedList::iterator() {
        return AbstractSequentialList::iterator();
    }

    int LinkedList::lastIndexOf(const shared<Object> o) {
        const auto* current = _tail;
        int index = _size - 1;

        while (current != nullptr) {
            if (current->data == nullptr) {
                if (o == nullptr)
                    return index;
                continue;
            }
            if (current->data->equals(o))
                return index;

            current = current->previous;
            index--;
        }

        return -1;
    }

    class LinkedList_ListIterator final : public virtual Object, public virtual ListIterator {
        shared<LinkedList> parent;
        int index; ///< represents the index @function next would return
        int modCount;

    public:
        LinkedList_ListIterator(const shared<LinkedList> &parent, const int index) : parent(parent), index(index),
            modCount(parent->modCount) {}

        void add(const shared<Object> e) override {
            parent->add(index, e);
        }

        bool hasNext() override {
            return index != parent->_size;
        }

        bool hasPrevious() override {
            return index - 1 >= 0;
        }

        shared<Object> next() override {
            if (index < parent->_size)
                return parent->get(index++);
            throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION");
        }

        int nextIndex() override {
            return index;
        }

        shared<Object> previous() override {
            if (index >= 0)
                return parent->get(index--);
            throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION");
        }

        int previousIndex() override {
            return index - 1;
        }

        void remove() override {
            parent->remove(index);
        }

        void set(const shared<Object> e) override {
            parent->set(index, e);
        }
    };

    shared<ListIterator> LinkedList::listIterator(int index) {
        if (index < 0 || index > _size)
            throw std::runtime_error("THROW INDEXOUTOFBOUNDSEXCEPTION");
        return alloc<LinkedList_ListIterator>(std::dynamic_pointer_cast<LinkedList>(shared_from_this()), index);
    }

    shared<ListIterator> LinkedList::listIterator() {
        return alloc<LinkedList_ListIterator>(std::dynamic_pointer_cast<LinkedList>(shared_from_this()), 0);
    }

    bool LinkedList::offer(const shared<Object> e) {
        if (_size == 0) {
            auto* new_tail = new linked_list_node{nullptr, e, nullptr};
            _head = _tail = new_tail;
            _size++;
            return true;
        }

        const auto new_tail = new linked_list_node{_tail, e, nullptr};
        _tail->next = new_tail;
        _tail = new_tail;
        _size++;

        return true;
    }

    bool LinkedList::offerFirst(const shared<Object> e) {
        if (_size == 0) {
            auto* new_head = new linked_list_node{nullptr, e, nullptr};
            _head = _tail = new_head;
            _size++;
            return true;
        }

        const auto new_head = new linked_list_node{nullptr, e, _head};
        _head->previous = new_head;
        _head = new_head;
        _size++;

        return true;
    }

    bool LinkedList::offerLast(const shared<Object> e) {
        if (_size == 0) {
            auto* new_tail = new linked_list_node{nullptr, e, nullptr};
            _head = _tail = new_tail;
            _size++;
            return true;
        }

        const auto new_tail = new linked_list_node{_tail, e, nullptr};
        _tail->next = new_tail;
        _tail = new_tail;
        _size++;

        return true;
    }

    shared<Object> LinkedList::peek() {
        if (_size == 0)
            return nullptr;

        return _head->data;
    }

    shared<Object> LinkedList::peekFirst() {
        if (_size == 0)
            return nullptr;

        return _head->data;
    }

    shared<Object> LinkedList::peekLast() {
        if (_size == 0)
            return nullptr;

        return _tail->data;
    }

    shared<Object> LinkedList::poll() {
        if (_size == 0)
            return nullptr;

        auto current = _head->data;

        _head = _head->next;
        delete _head->previous;
        _head->previous = nullptr;

        _size--;

        return current;
    }

    shared<Object> LinkedList::pollFirst() {
        if (_size == 0)
            return nullptr;

        auto current = _head->data;

        _head = _head->next;
        delete _head->previous;
        _head->previous = nullptr;

        _size--;

        return current;
    }

    shared<Object> LinkedList::pollLast() {
        if (_size == 0)
            return nullptr;

        auto current = _tail->data;

        _tail = _tail->previous;
        delete _tail->next;
        _tail->next = nullptr;

        _size--;

        return current;
    }

    shared<Object> LinkedList::pop() {
        return removeFirst();
    }

    void LinkedList::push(const shared<Object> e) {
        addFirst(e);
    }

    shared<Object> LinkedList::remove() {
        checkNotEmpty();

        auto current = _head->data;

        _head = _head->next;
        delete _head->previous;
        _head->previous = nullptr;

        _size--;

        return current;
    }

    shared<Object> LinkedList::remove(const int index) {
        checkBounds(index);

        linked_list_node* current;
        if (index == 0) {
            current = _head;
            _head = _head->next;
            _head->previous = nullptr;
            auto obj = current->data;
            delete current;
            _size--;
            return obj;
        }
        if (index == _size - 1) {
            current = _tail;
            _tail = _tail->previous;
            _tail->next = nullptr;
            auto obj = current->data;
            delete current;
            _size--;
            return obj;
        }

        current = _head;
        for (int i = 0; i < index; i++)
            current = current->next;

        auto* next = current->next;
        auto* prev = current->previous;
        prev->next = next;
        next->previous = prev;
        auto obj = current->data;
        delete current;
        _size--;
        return obj;
    }

    bool LinkedList::remove(const shared<Object> o) {
        if (_size == 0)
            return false;

        const auto* current = _head;

        while (current != nullptr) {
            if (current->data == nullptr) {
                if (o == nullptr)
                    goto rm;
                continue;
            }
            if (current->data->equals(o))
                goto rm;

            current = current->next;
        }

        return false;

    rm:
        const auto prev = current->previous;
        const auto next = current->next;
        prev->next = next;
        next->previous = prev;
        delete current;
        _size--;

        return true;
    }

    shared<Object> LinkedList::removeFirst() {
        checkNotEmpty();

        auto previous = _head->data;

        const auto lasthead = _head;
        _head = _head->next;
        delete lasthead;
        _head->previous = nullptr;

        _size--;

        return previous;
    }

    bool LinkedList::removeFirstOccurrence(const shared<Object> o) {
        const auto* current = _head;

        while (current != nullptr) {
            if (current->data == nullptr) {
                if (o == nullptr)
                    goto found;
                continue;
            }
            if (current->data->equals(o))
                goto found;

            current = current->next;
        }
        return false;

    found:
        if (current == _head) {
            removeFirst();
        } else if (current == _tail) {
            removeLast();
        } else {
            auto* next = current->next;
            auto* prev = current->previous;
            prev->next = next;
            next->previous = prev;
            delete current;
        }

        return true;
    }

    shared<Object> LinkedList::removeLast() {
        checkNotEmpty();

        auto previous = _tail->data;

        const auto lasttail = _tail;
        _tail = _tail->previous;
        delete lasttail;
        _tail->next = nullptr;

        _size--;

        return previous;
    }

    bool LinkedList::removeLastOccurrence(const shared<Object> o) {
        const auto* current = _tail;

        while (current != nullptr) {
            if (current->data == nullptr) {
                if (o == nullptr)
                    goto found;
                continue;
            }
            if (current->data->equals(o))
                goto found;

            current = current->previous;
        }
        return false;

        found:
            if (current == _head) {
                removeFirst();
            } else if (current == _tail) {
                removeLast();
            } else {
                auto* next = current->next;
                auto* prev = current->previous;
                prev->next = next;
                next->previous = prev;
                delete current;
            }

        return true;
    }

    shared<Object> LinkedList::set(const int index, const shared<Object> element) {
        checkBounds(index);

        auto* current = _head;
        for (int i = 0; i < index; i++)
            current = current->next;

        auto old = current->data;
        current->data = element;

        return old;
    }

    int LinkedList::size() {
        return _size;
    }

    class LinkedList_Spliterator_Specialization final : public virtual Object, public virtual Spliterator {
        shared<LinkedList> parent;
        int index = 0;
        int limit;

    public:
        explicit LinkedList_Spliterator_Specialization(const shared<LinkedList> &parent) : parent(parent), limit(parent->size()) {}

        explicit LinkedList_Spliterator_Specialization(const shared<LinkedList> &parent, const int index, const int limit)
          : parent(parent), index(index), limit(limit) {}

        int characteristics() override {
            return SIZED | SUBSIZED | ORDERED;
        }

        long long estimateSize() override {
            return limit - index;
        }

        bool tryAdvance(const shared<function::Consumer> action) override {
            if (index + 1 <= limit) {
                action->accept(parent->get(index++));
                return true;
            }
            return false;
        }

        shared<Spliterator> trySplit() override;
    };

    shared<Spliterator> LinkedList::spliterator() {
        return alloc<LinkedList_Spliterator_Specialization>(std::dynamic_pointer_cast<LinkedList>(shared_from_this()), 0, _size);
    }

    shared<Array<>> LinkedList::toArray() {
        shared<Array<>> array = alloc<Array<>>(_size);

        const auto* current = _head;
        for (int i = 0; i < _size; i++) {
            array->data[i] = current->data;
            current = current->next;
        }

        return array;
    }

    shared<Array<>> LinkedList::toArray(const shared<function::IntFunction> generator) {
        return AbstractSequentialList::toArray(generator);
    }

    shared<Array<>> LinkedList::toArray(const shared<Array<>> a) {
        shared<Array<>> array = a;

        array->resize(_size);

        const auto* current = _head;
        for (int i = 0; i < _size; i++) {
            array->data[i] = current->data;
            current = current->next;
        }

        return array;
    }
}
