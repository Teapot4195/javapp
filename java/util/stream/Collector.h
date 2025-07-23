#pragma once

#include <Enum.h>
#include <Object.h>

namespace java::util {
    namespace function {
        class BiConsumer;
        class BinaryOperator;
        class Function;
        class Supplier;
    }
    class Set;
}

namespace java::util::stream {
    class Collector : public virtual Object {
    public:
        ~Collector() override = default;

        struct Characteristics : virtual Object, virtual Enum {
            static shared<Characteristics> CONCURRENT, IDENTITY_FINISH, UNORDERED;

            using Enum::Enum;
        };

        virtual shared<function::BiConsumer> accumulator() = 0;

        virtual shared<Set> characteristics() = 0;

        virtual shared<function::BinaryOperator> combiner() = 0;

        virtual shared<function::Function> finisher() = 0;

        static shared<Collector> of(const shared<function::Supplier>& supplier, const shared<function::BiConsumer>& accumulator,
            const shared<function::BinaryOperator>& combiner, const shared<function::Function>& finisher,
            const std::vector<shared<Characteristics>>& characteristics);

        static shared<Collector> of(const shared<function::Supplier>& supplier, const shared<function::BiConsumer>& accumulator,
            const shared<function::BinaryOperator>& combiner, const std::vector<shared<Characteristics>>& characteristics);

        virtual shared<function::Supplier> supplier() = 0;
    };
}
