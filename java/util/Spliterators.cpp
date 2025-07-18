#include "Spliterators.h"

#include "Spliterator.h"

namespace java::util {
    class Spliterator_Empty_Specialization final : public virtual Spliterator {
    public:
        ~Spliterator_Empty_Specialization() override = default;

        int characteristics() override {
            return SIZED | SUBSIZED;
        }

        long long estimateSize() override {
            return 0;
        }

        bool tryAdvance(shared<function::Consumer> action) override {
            return false;
        }

        shared<Spliterator> trySplit() override {
            return nullptr;
        }
    };

    shared<Spliterator> Spliterators::emptySpliterator() {
        return alloc<Spliterator_Empty_Specialization>();
    }
}
