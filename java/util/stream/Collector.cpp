#include "Collector.h"

#include <unordered_set>

#include "java/util/Set.h"

namespace java::util::stream {
    shared<Collector::Characteristics> Collector::Characteristics::CONCURRENT = alloc<Characteristics>("CONCURRENT");
    shared<Collector::Characteristics> Collector::Characteristics::IDENTITY_FINISH = alloc<Characteristics>("IDENTITY_FINISH");
    shared<Collector::Characteristics> Collector::Characteristics::UNORDERED = alloc<Characteristics>("UNORDERED");

    class Collector_Specialization final : public virtual Object, public virtual Collector {
        shared<function::Supplier> _supplier;
        shared<function::BiConsumer> _accumulator;
        shared<function::BinaryOperator> _combiner;
        shared<function::Function> _finisher;
        shared<Set> _characteristics;

    public:
        Collector_Specialization(const shared<function::Supplier> &supplier,
            const shared<function::BiConsumer> &accumulator, const shared<function::BinaryOperator> &combiner,
            const shared<function::Function> &finisher, const std::vector<shared<Characteristics>> &characteristics)
          : _supplier(supplier), _accumulator(accumulator),
            _combiner(combiner), _finisher(finisher) {
            _characteristics = Set::of(decay_vec(characteristics));
        }

        shared<function::Supplier> supplier() override {
            return _supplier;
        }

        shared<function::BiConsumer> accumulator() override {
            return _accumulator;
        }

        shared<function::BinaryOperator> combiner() override {
            return _combiner;
        }

        shared<function::Function> finisher() override {
            return _finisher;
        }

        shared<Set> characteristics() override {
            return _characteristics;
        }
    };

    shared<Collector> Collector::of(const shared<function::Supplier>& supplier,
        const shared<function::BiConsumer>& accumulator, const shared<function::BinaryOperator>& combiner,
        const shared<function::Function>& finisher, const std::vector<shared<Characteristics>>& characteristics) {
        return alloc<Collector_Specialization>(supplier, accumulator, combiner, finisher, characteristics);
    }

    shared<Collector> Collector::of(const shared<function::Supplier>& supplier,
        const shared<function::BiConsumer>& accumulator, const shared<function::BinaryOperator>& combiner,
        const std::vector<shared<Characteristics>>& characteristics) {
        return alloc<Collector_Specialization>(supplier, accumulator, combiner, nullptr, characteristics);
    }
}
