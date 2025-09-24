#include "ArrayList.h"

#include <Comparable.h>

#include "Iterator.h"
#include "Objects.h"
#include "Spliterator.h"
#include "function/Consumer.h"
#include "function/Predicate.h"

#include <cstring>
#include <java/util/Comparator.h>

// TODO: implementor's note, we can probably slab allocate a large block of memory from a custom class, this allows
// us to avoid a lot of alloc delete calls (probably). /shrug

namespace java::util {
    void ArrayList::copyFrom(ArrayList *other) {
        if (other->_capacity != _capacity)
            throw std::runtime_error("Sizes aren't equal! This is a massive implementation issue");

        std::copy_n(other->_array, other->size(), _array);

        _size = other->_size;
    }

    void ArrayList::checkBounds(const int index) const {
        if (index < 0 || index >= _size)
            throw std::runtime_error("THROW INDEXOUTOFBOUNDSEXCEPTION");
    }

    void ArrayList::resize(const int newSize) {
        if (_array == nullptr) {
            _array = new shared<Object>[newSize];
            _capacity = newSize;
            return;
        }
        auto* newArray = new shared<Object>[newSize];

        std::copy_n(_array, _size, newArray);

        delete[] _array;

        _array = newArray;
        _capacity = newSize;
    }

    ArrayList::~ArrayList() {
        if (_capacity > 0)
            delete[] _array;
    }

    ArrayList::ArrayList() : ArrayList(10) {}

    ArrayList::ArrayList(const int initialCapacity) : _array(new shared<Object>[initialCapacity]), _size(0),
        _capacity(initialCapacity) {}

    ArrayList::ArrayList(const shared<Collection> &c) {
        _array = new shared<Object>[c->size()];
        _size = c->size();
        _capacity = c->size();

        int index = 0;
        const auto it = c->iterator();
        while (it->hasNext() && index < _size) {
            _array[index++] = it->next();
        }

        if (it->hasNext() || index != _size) {
            throw std::runtime_error("size is inaccurate?");
        }
    }

    void ArrayList::add(const int index, const shared<Object> element) {
        checkBounds(index);

        // TODO: this is potentially 2 copies, let's make it one if possible
        if (_size + 1 >= _capacity)
            resize(2 * _capacity);

        std::copy_backward(_array + index, _array + _size, _array + 1 + _size);

        _array[index] = element;

        _size++;
    }

    bool ArrayList::add(const shared<Object> e) {
        if (_size == _capacity) {
            ensureCapacity(_size * 2); // double size
        }
        _array[_size++] = e;
        return true;
    }

    bool ArrayList::addAll(const int index, const shared<Collection> c) {
        checkBounds(index);
        const auto it = c->iterator();

        ensureCapacity(size() + c->size());
        std::copy_backward(_array + index, _array + _size, _array + _size + c->size());

        int i = 0;
        while (it->hasNext()) {
            _array[index + (i++)] = it->next();
        }

        _size += c->size();

        return true;
    }

    bool ArrayList::addAll(const shared<Collection> c) {
        if (c == nullptr)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION");

        ensureCapacity(c->size());

        const auto it = c->iterator();

        while (it->hasNext()) {
            add(it->next());
        }

        return true;
    }

    void ArrayList::clear() {
        delete[] _array;
        _array = nullptr;
        _size = 0;
        _capacity = 0;
    }

    std::shared_ptr<Object> ArrayList::clone() {
        auto obj = alloc<ArrayList>(_capacity);
        obj->copyFrom(this);
        return obj;
    }

    bool ArrayList::contains(const shared<Object> o) {
        for (int i = 0; i < _size; i++) {
            if (Objects::equals(o, _array[i]))
                return true;
        }

        return false;
    }

    void ArrayList::ensureCapacity(const int minCapacity) {
        if (_capacity < minCapacity) {
            resize(minCapacity);
        }
    }

    bool ArrayList::equals(Object *obj) {
        if (obj == this)
            return true;
        if (!obj->instanceof<List>())
            return false;
        auto* other = dynamic_cast<List *>(obj);
        if (other->size() != _size)
            return false;

        const auto it = other->iterator();

        for (int i = 0; i < _size; i++) {
            if (!it->hasNext())
                return false;
            auto onext = it->next();
            auto inext = _array[i];

            if (inext == nullptr ? onext != nullptr : !inext->equals(onext))
                return false;
        }

        if (it->hasNext())
            return false;

        return true;
    }

    void ArrayList::forEach(const shared<function::Consumer> action) {
        for (int i = 0; i < _size; i++) {
            action->accept(_array[i]);
        }
    }

    shared<Object> ArrayList::get(const int index) {
        checkBounds(index);
        return _array[index];
    }

    int ArrayList::hashCode() {
        int hashCode = 1;
        const auto it = iterator();

        while (it->hasNext()) {
            auto e = it->next();
            hashCode = 31 * hashCode + (e == nullptr ? 0 : e->hashCode());
        }

        return hashCode;
    }

    int ArrayList::indexOf(const shared<Object> o) {
        for (int i = 0; i < _size; i++) {
            if (Objects::equals(o, _array[i]))
                return i;
        }

        return -1;
    }

    bool ArrayList::isEmpty() {
        return _size == 0;
    }

    shared<Iterator> ArrayList::iterator() {
        return AbstractList::iterator();
    }

    int ArrayList::lastIndexOf(const shared<Object> o) {
        for (int i = _size - 1; i >= 0; i--) {
            if (Objects::equals(o, _array[i]))
                return i;
        }

        return -1;
    }

    shared<ListIterator> ArrayList::listIterator() {
        return AbstractList::listIterator();
    }

    shared<ListIterator> ArrayList::listIterator(const int index) {
        checkBounds(index);

        return AbstractList::listIterator(index);
    }

    shared<Object> ArrayList::remove(const int index) {
        checkBounds(index);

        auto prev = _array[index];

        std::copy(_array + index + 1, _array + _size, _array + index);

        _size--;

        return prev;
    }

    bool ArrayList::remove(const shared<Object> o) {
        for (int i = 0; i < _size; i++) {
            if (Objects::equals(o, _array[i])) {
                std::copy(_array + i + 1, _array + _size, _array + i);
                return true;
            }
        }

        _size--;

        return false;
    }

    bool ArrayList::removeAll(const shared<Collection> c) {
        const auto it = c->iterator();

        bool changed = false;

        while (it->hasNext()) {
            remove(it->next());
            changed = true;
        }

        return changed;
    }

    bool ArrayList::removeIf(const shared<function::Predicate> filter) {
        if (filter == nullptr)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION");

        int newSize = 0;
        for (int i = 0, head = 0; i < _size; i++) {
            if (!filter->test(_array[i])) {
                newSize++;

                if (head != i)
                    _array[head++] = _array[i];
                else
                    head++;
            }
        }

        std::swap(_size, newSize);
        return _size != newSize;
    }

    void ArrayList::removeRange(const int fromIndex, const int toIndex) {
        checkBounds(fromIndex);
        checkBounds(toIndex);
        if (toIndex < fromIndex)
            throw std::runtime_error("THROW INDEXOUTOFBOUNDSEXCEPTION");

        if (toIndex == fromIndex)
            return;

        const int sChange = toIndex - fromIndex;

        if (toIndex != _size) {
            // not last index, means we need to shift over
            std::copy(_array + toIndex, _array + _size, _array + fromIndex);
        }

        _size -= sChange;
    }

    bool ArrayList::retainAll(const shared<Collection> c) {
        if (c == nullptr)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION");

        int newSize = 0;
        for (int i = 0, head = 0; i < _size; i++) {
            if (c->contains(_array[i])) {
                newSize++;

                if (head != i)
                    _array[head++] = _array[i];
                else
                    head++;
            }
        }

        std::swap(_size, newSize);
        return _size != newSize;
    }

    shared<Object> ArrayList::set(const int index, const shared<Object> element) {
        checkBounds(index);
        auto prev = _array[index];
        _array[index] = element;
        return prev;
    }

    int ArrayList::size() {
        return _size;
    }

    class ArrayList_Spliterator_Specialization final : public virtual Object, public virtual Spliterator {
        shared<ArrayList> parent;
        int index = 0;
        int limit;

    public:
        explicit ArrayList_Spliterator_Specialization(const shared<ArrayList> &parent) : parent(parent), limit(parent->size()) {}

        explicit ArrayList_Spliterator_Specialization(const shared<ArrayList> &parent, const int index, const int limit)
          : parent(parent), index(index), limit(limit) {}

        int characteristics() override {
            return SIZED | SUBSIZED | ORDERED;
        }

        long long estimateSize() override {
            return limit - index;
        }

        bool tryAdvance(const shared<function::Consumer> action) override {
            if (index + 1 <= limit) {
                action->accept(parent->_array[index++]);
                return true;
            }
            return false;
        }

        shared<Spliterator> trySplit() override;
    };

    shared<Spliterator> ArrayList_Spliterator_Specialization::trySplit() {
        if (index == limit)
            return nullptr; // nothing to split
        if (limit - index == 1)
            return nullptr; // can't split
        int partition = (index + limit) / 2;
        index = partition;
        return alloc<ArrayList_Spliterator_Specialization>(parent, index, partition);
    }

    shared<Spliterator> ArrayList::spliterator() {
        return alloc<ArrayList_Spliterator_Specialization>(std::dynamic_pointer_cast<ArrayList>(shared_from_this()));
    }

    shared<List> ArrayList::subList(const int fromIndex, const int toIndex) {
        return AbstractList::subList(fromIndex, toIndex);
    }

    shared<Array<>> ArrayList::toArray() {
        auto arr = alloc<Array<>>(_size);

        std::copy_n(_array, _size, arr->data);
        arr->length = _size;

        return arr;
    }

    shared<Array<>> ArrayList::toArray(const shared<function::IntFunction> generator) {
        return AbstractList::toArray(generator);
    }

    shared<Array<>> ArrayList::toArray(const shared<Array<>> a) {
        auto arr = a;

        if (arr->length < _size) {
            arr->resize(_size);
        }

        std::copy_n(_array, _size, arr->data);

        if (arr->length > _size) {
            for (int i = _size; i < arr->length; i++)
                arr->data[i] = nullptr;
        }

        return arr;
    }

    void ArrayList::trimToSize() {
        auto* newArray = new shared<Object>[_size];

        std::copy_n(_array, _size, newArray);

        delete[] _array;

        _array = newArray;
    }

    void ArrayList::sort() {
        shared<Comparable> *a = new shared<Comparable>[_size], *work = new shared<Comparable>[_size];

        for (int i = 0; i < _size; i++)
            work[i] = a[i] = std::dynamic_pointer_cast<Comparable>(_array[i]);

        MergeSort(a, work, _size);

        delete[] work;

        std::copy_n(a, _size, _array);

        delete[] a;
    }

    void ArrayList::sort(shared<Comparator> c) {
        shared<Object> *a = new shared<Object>[_size], *work = new shared<Object>[_size];

        for (int i = 0; i < _size; i++)
            work[i] = a[i] = _array[i];

        MergeSortComparator(a, work, _size, c);

        delete[] work;

        std::copy_n(a, _size, _array);

        delete[] a;
    }

    void ArrayList::MergeSort(shared<Comparable> *a, shared<Comparable> *work, size_t n) {
        SplitMerge(a, 0, n, work);
    }

    void ArrayList::SplitMerge(shared<Comparable> *b, size_t iBegin, size_t iEnd, shared<Comparable> *a) {
        if (iEnd - iBegin <= 1)
            return;

        size_t iMiddle = (iEnd + iBegin) / 2;

        SplitMerge(a, iBegin, iMiddle, b);
        SplitMerge(a, iMiddle, iEnd, b);

        Merge(b, iBegin, iMiddle, iEnd, a);
    }

    void ArrayList::Merge(shared<Comparable> *b, size_t iBegin, size_t iMiddle, size_t iEnd, shared<Comparable> *a) {
        auto i = iBegin, j = iEnd;

        for (auto k = iBegin; k < iEnd; k++) {
            if (i < iMiddle && (j >= iEnd || a[i]->compareTo(a[j]) <= 0))
                b[k] = a[i++];
            else
                b[k] = a[j++];
        }
    }

    void ArrayList::MergeSortComparator(shared<Object> *a, shared<Object> *work, size_t n,
        shared<Comparator> comparator) {
        SplitMergeComparator(a, 0, n, work, comparator);
    }

    void ArrayList::SplitMergeComparator(shared<Object> *b, size_t iBegin, size_t iEnd, shared<Object> *a,
    shared<Comparator> comparator) {
        if (iEnd - iBegin <= 1)
            return;

        size_t iMiddle = (iEnd + iBegin) / 2;

        SplitMergeComparator(a, iBegin, iMiddle, b, comparator);
        SplitMergeComparator(a, iMiddle, iEnd, b, comparator);

        MergeComparator(b, iBegin, iMiddle, iEnd, a, comparator);
    }

    void ArrayList::MergeComparator(shared<Object> *b, size_t iBegin, size_t iMiddle, size_t iEnd, shared<Object> *a,
    shared<Comparator> comparator) {
        auto i = iBegin, j = iEnd;

        for (auto k = iBegin; k < iEnd; k++) {
            if (i < iMiddle && (j >= iEnd || comparator->compare(a[i], a[j]) <= 0))
                b[k] = a[i++];
            else
                b[k] = a[j++];
        }
    }
}
