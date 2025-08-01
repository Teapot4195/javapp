#pragma once

#include <Object.h>

namespace java::util {
    namespace function {
        class Function;
        class ToDoubleFunction;
        class ToIntFunction;
        class ToLongFunction;
    }

    class Comparator : public virtual Object {
        std::function<int(shared<Object>, shared<Object>)> inner;

    protected:
        Comparator() = default;

    public:
        ~Comparator() override = default;

        /**
         * @brief Implicit conversion function
         * @tparam F the type of the function
         * @param func the function
         */
        template <typename F>
        explicit Comparator(F&& func) : inner(std::forward<F>(func)) {}

        virtual int compare(shared<Object> o1, shared<Object> o2);

        static shared<Comparator> comparing(const shared<function::Function> &keyExtractor);

        static shared<Comparator> comparing(const shared<function::Function> &keyExtractor, const shared<Comparator> &keyComparator);

        static shared<Comparator> comparingDouble(const shared<function::ToDoubleFunction> &keyExtractor);

        static shared<Comparator> comparingInt(const shared<function::ToIntFunction> &keyExtractor);

        static shared<Comparator> comparingLong(const shared<function::ToLongFunction> &keyExtractor);

        static shared<Comparator> naturalOrder();

        static shared<Comparator> nullsFirst(const shared<Comparator> &comparator);

        static shared<Comparator> nullsLast(const shared<Comparator> &comparator);

        virtual shared<Comparator> reversed();

        static shared<Comparator> reverseOrder();

        virtual shared<Comparator> thenComparing(shared<Comparator> other);

        virtual shared<Comparator> thenComparing(shared<function::Function> keyExtractor);

        virtual shared<Comparator> thenComparing(shared<function::Function> keyExtractor, shared<Comparator> keyComparator);

        virtual shared<Comparator> thenComparingDouble(shared<function::ToDoubleFunction> keyExtractor);

        virtual shared<Comparator> thenComparingInt(shared<function::ToIntFunction> keyExtractor);

        virtual shared<Comparator> thenComparingLong(shared<function::ToLongFunction> keyExtractor);
    };
}
