#include "HashSet.h"

#include <SwissTableHashSet.h>
#include <java/util/Iterator.h>
#include <java/util/Spliterator.h>
#include <java/util/function/Consumer.h>

namespace java::util {
    class HashSet_Iterator_Specialization final : public virtual Object, public virtual java::util::Iterator {
        shared<HashSet> set;
        size_t data;
        long long count;

    public:
        explicit HashSet_Iterator_Specialization(const shared<HashSet> &parent) : set(parent), data(0), count(0) {}

        bool hasNext() override {
            return count < set->table->table_size;
        }

        shared<Object> next() override {
            if (count < set->table->table_size)
                throw std::runtime_error("THROW NOSUCHELEMENTEXCEPTION!");
            for (size_t i = data; i < set->table->table_size; i++) {
                if (set->table->metadata[i].usage && !set->table->metadata[i].tombstone) {
                    data = i + 1;
                    count++;
                    return set->table->table[i].key;
                }
            }

            panic("Unreachable code!");
        }
    };

    class HashSet_Spliterator_Specialization final : public virtual Object, public virtual java::util::Spliterator {
        shared<HashSet> set;
        size_t start, end;

    public:
        explicit HashSet_Spliterator_Specialization(const shared<HashSet> &parent) : set(parent), start(0), end(parent->table->bucket_count) {}

        HashSet_Spliterator_Specialization(const shared<HashSet> &parent, size_t start, size_t end): set(parent), start(start), end(end) {}

        int characteristics() override {
            return SIZED | DISTINCT;
        }

        long long estimateSize() override {
            return static_cast<long long>(end - start);
        }

        bool tryAdvance(shared<java::util::function::Consumer> action) override {
            for (size_t i = start; i < end; i++) {
                if (set->table->metadata[i].usage && !set->table->metadata[i].tombstone) {
                    start = i + 1;
                    action->accept(set->table->table[i].key);
                    return true;
                }
            }

            return false;
        }

        shared<Spliterator> trySplit() override;
    };

    shared<java::util::Spliterator> HashSet_Spliterator_Specialization::trySplit() {
        if (end - start == 1) {
            // cannot split.
            return null;
        }

        size_t e = this->end / 2;
        shared<Spliterator> result = alloc<HashSet_Spliterator_Specialization>(set, start, e);
        start = e;

        return result;
    }

    HashSet::HashSet() {
        table = new SwissTableHashSet();
    }

    HashSet::HashSet(const int initialCapacity) {
        table = new SwissTableHashSet(initialCapacity);
    }

    HashSet::HashSet(const int initialCapacity, const float loadFactor) {
        table = new SwissTableHashSet(initialCapacity, loadFactor);
    }

    HashSet::HashSet(const shared<Collection> &collection) {
        table = new SwissTableHashSet(collection);
    }

    HashSet::~HashSet() {
        delete table;
    }

    bool HashSet::add(shared<Object> e) {
        return table->table_insert_or_set(e);
    }

    void HashSet::clear() {
        table->table_clear();
    }

    std::shared_ptr<Object> HashSet::clone() {
        shared<HashSet> set = alloc<HashSet>();

        set->table = table->clone();

        return set;
    }

    bool HashSet::contains(const shared<Object> o) {
        return table->table_get(o);
    }

    bool HashSet::isEmpty() {
        return table->table_size == 0;
    }

    shared<java::util::Iterator> HashSet::iterator() {
        return alloc<HashSet_Iterator_Specialization>(pself);
    }

    shared<HashSet> HashSet::newHashSet(int numElements) {
        return alloc<HashSet>(numElements);
    }

    bool HashSet::remove(shared<Object> o) {
        return table->table_delete(o);
    }

    int HashSet::size() {
        return static_cast<int>(table->table_size);
    }

    shared<java::util::Spliterator> HashSet::spliterator() {
        return AbstractSet::spliterator();
    }

    shared<Array<>> HashSet::toArray() {
        shared<Array<>> array = alloc<Array<>>();
        return toArray(array);
    }

    shared<Array<>> HashSet::toArray(shared<Array<>> a) {
        a->resize(static_cast<int>(table->table_size));
        int aindex = 0;
        for (size_t i = 0; i < table->table_size; i++) {
            if (table->metadata[i].usage && !table->metadata[i].tombstone)
                a->data[aindex++] = table->table[i].key;
        }

        return a;
    }

    shared<Array<>> HashSet::toArray(shared<java::util::function::IntFunction> generator) {
        return AbstractSet::toArray(generator);
    }
}
