#pragma once

#include <Cloneable.h>
#include <java/io/Serializable.h>
#include <java/util/AbstractSet.h>

class SwissTableHashSet;

namespace java::util {
    class HashSet : public virtual Object, public virtual AbstractSet, public virtual Cloneable,
                    public virtual io::Serializable {
    public:
        HashSet();

        explicit HashSet(int initialCapacity);

        HashSet(int initialCapacity, float loadFactor);

        explicit HashSet(const shared<Collection> &collection);

        ~HashSet() override;

        bool add(shared<Object> e) override;

        void clear() override;

        std::shared_ptr<Object> clone() override;

        bool contains(shared<Object> o) override;

        bool isEmpty() override;

        shared<Iterator> iterator() override;

        static shared<HashSet> newHashSet(int numElements);

        bool remove(shared<Object> o) override;

        int size() override;

        shared<Spliterator> spliterator() override;

        shared<Array<>> toArray() override;

        shared<Array<>> toArray(shared<Array<>> a) override;

        shared<Array<>> toArray(shared<function::IntFunction> generator) override;

    private:
        SwissTableHashSet* table;

        friend class HashSet_Iterator_Specialization;
        friend class HashSet_Spliterator_Specialization;
    };
}
